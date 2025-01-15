#include "lexer.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io/io.h"
#include "utils/utils.h"

/*
#####################################
Utilis
#####################################
*/

#define UNITIALIZED_CHAR (-5)

static int last_char(struct lexer *lexer)
{
    if (lexer->current_char == UNITIALIZED_CHAR) // read the next character
        lexer->current_char = reader_next(lexer->reader);
    return lexer->current_char;
}

static int next_char(struct lexer *lexer)
{
    lexer->current_char = reader_next(lexer->reader);
    return lexer->current_char;
}

// Skip the whitespace characters in the data
static void lexer_skip_whitespace(struct lexer *lexer)
{
    while (isspace(last_char(lexer)) && last_char(lexer) != '\n')
        next_char(lexer);
}

static void lexer_skip_comment(struct lexer *lexer)
{
    while (lexer->mode == LEXING_COMMENT && last_char(lexer) != '\n'
           && last_char(lexer) != EOF)
        next_char(lexer);
    lexer->mode = LEXING_NORMAL;
}

// Test if a character is a word character
static int lexer_is_word_char(struct lexer *lexer)
{
    const int c = last_char(lexer);
    if (c == EOF)
        return 0;
    static const char reserved[] = " \t\n;#|&<>";
    for (size_t i = 0; i < sizeof(reserved) - 1; i++)
        if (c == reserved[i])
            return 0;
    return 1;
}

#define BLIND_PUSH_WHEN(cond)                                                  \
    if (cond)                                                                  \
    {                                                                          \
        string_push(&word->value, last_char(lexer));                           \
        next_char(lexer);                                                      \
        lexer->escape_next = 0;                                                \
        continue;                                                              \
    }

// Returns true if it is a redirection and adds the redirection to the word
static int lexer_try_redir(struct lexer *lexer, struct word *word)
{
    char c = (char)last_char(lexer);
    if (c != '>' && c != '<')
        return 0;
    // check if word is a number
    for (size_t i = 0; i < word->value.length; i++)
        if (!isdigit(word->value.data[i]))
            return 0;

    string_push(&word->value, c);
    next_char(lexer);

    // check if the next character completes the redirection
    char c2 = (char)last_char(lexer);
    char str[] = { c, c2, '\0' };
    if (strcmp(str, ">>") == 0 || strcmp(str, "<<") == 0
        || strcmp(str, "<>") == 0 || strcmp(str, ">&") == 0
        || strcmp(str, "<&") == 0 || strcmp(str, ">|") == 0)
    {
        string_push(&word->value, c2);
        next_char(lexer);
    }

    return 1;
}

static int lexer_is_name_char(struct lexer *lexer)
{
    const int c = last_char(lexer);
    return isalnum(c) || c == '_';
}

// returns true and populates var if it parsed a variable, false otherwise
int lexer_lex_variable(struct lexer *lexer, struct variable *var)
{
    if (last_char(lexer) == EOF)
        return 0;
    static const char accepted_single[] = "$*!@#?-";
    const char c = (char)last_char(lexer);
    // try special vars first
    for (size_t i = 0; i < sizeof(accepted_single) - 1; i++)
    {
        if (c == accepted_single[i])
        {
            string_init(&var->name);
            string_push(&var->name, c);
            next_char(lexer);
            return 1;
        }
    }

    // try normal vars
    if (!lexer_is_name_char(lexer))
        return 0;
    string_init(&var->name);
    while (lexer_is_name_char(lexer))
    {
        string_push(&var->name, (char)last_char(lexer));
        next_char(lexer);
    }
    return 1;
}

// Create a word token or a keyword token
static struct token lexer_next_handle_word(struct lexer *lexer)
{
    struct word *word = word_new();
    int has_escaped = 0;
    while (lexer_is_word_char(lexer) || lexer->escape_next
           || lexer->mode == LEXING_QUOTED
           || lexer->mode == LEXING_DOUBLE_QUOTED)
    {
        // has_escaped can only go from 0 to 1 and never back to 0 thanks to |=
        has_escaped |= lexer->escape_next || lexer->mode == LEXING_QUOTED
            || lexer->mode == LEXING_DOUBLE_QUOTED;
        char c = (char)last_char(lexer);

        BLIND_PUSH_WHEN(lexer->escape_next);

        if ((lexer->mode == LEXING_QUOTED && c == '\'')
            || (lexer->mode == LEXING_DOUBLE_QUOTED && c == '"'))
        {
            lexer->mode = LEXING_NORMAL;
            next_char(lexer);
            continue;
        }

        BLIND_PUSH_WHEN(lexer->mode == LEXING_QUOTED);
        if (c == '$')
        {
            next_char(lexer);
            struct variable var;
            if (lexer_lex_variable(lexer, &var))
            {
                var.pos = word->value.length;
                word_push_variable(word, var);
            }
            else
                string_push(&word->value, c);
            continue;
        }
        BLIND_PUSH_WHEN(lexer->mode == LEXING_DOUBLE_QUOTED);

        if (c == '\\' && lexer->mode == LEXING_NORMAL)
        {
            lexer->escape_next = 1;
            next_char(lexer);
            continue;
        }

        if (c == '\'' || c == '"')
        {
            lexer->mode = c == '\'' ? LEXING_QUOTED : LEXING_DOUBLE_QUOTED;
            next_char(lexer);
            continue;
        }

        BLIND_PUSH_WHEN(1);
    }

    struct token token;
    token.type = TOKEN_WORD;
    token.has_escaped = has_escaped;

    if (!has_escaped && lexer_try_redir(lexer, word))
        token.type = TOKEN_REDIR;

    token.word = word;
    return token;
}

// Switch the token based on the character
static struct token lexer_switch(struct lexer *lexer)
{
    struct token token;
    token.word = NULL;
    switch (last_char(lexer))
    {
    case '\n':
        token.type = TOKEN_EOL;
        break;
    case ';':
        token.type = TOKEN_SEMICOLON;
        break;
    case '\'':
        lexer->mode = LEXING_QUOTED;
        lexer->current_char = UNITIALIZED_CHAR;
        return lexer_next_handle_word(lexer);
    case '"':
        lexer->mode = LEXING_DOUBLE_QUOTED;
        lexer->current_char = UNITIALIZED_CHAR;
        return lexer_next_handle_word(lexer);
    case '#':
        lexer->mode = LEXING_COMMENT;
        lexer->current_char = UNITIALIZED_CHAR;
        return lexer_next(lexer);
    case '>':
    case '<':
        // redirections are handled in lexer_next_handle_word
        return lexer_next_handle_word(lexer);
    default:
        token.type = TOKEN_UNKNOWN;
        break;
    }
    lexer->current_char = UNITIALIZED_CHAR;
    return token;
}

// Exports

struct lexer *lexer_new(struct reader *reader)
{
    struct lexer *lexer = malloc(sizeof(struct lexer));
    if (!lexer)
    {
        fprintf(stderr, "Failed allocating memory for lexer\n");
        return NULL;
    }
    lexer->reader = reader;
    lexer->current.type = TOKEN_UNSET;
    lexer->current.word = NULL;
    lexer->current_char = UNITIALIZED_CHAR;
    lexer->escape_next = 0;
    lexer->mode = LEXING_NORMAL;

    return lexer;
}

void lexer_free(struct lexer *lexer)
{
    free(lexer);
}

struct token lexer_next(struct lexer *lexer)
{
    lexer_skip_whitespace(lexer);
    lexer_skip_comment(lexer);

    if (last_char(lexer) == EOF)
    {
        struct token token;
        token.type = TOKEN_EOF;
        token.word = NULL;
        return token;
    }

    if (lexer_is_word_char(lexer) || lexer->escape_next)
        return lexer_next_handle_word(lexer);

    return lexer_switch(lexer);
}

struct token lexer_peek(struct lexer *lexer)
{
    if (lexer->current.type != TOKEN_UNSET)
        return lexer->current;
    lexer->current = lexer_next(lexer);
    return lexer->current;
}

struct token lexer_pop(struct lexer *lexer)
{
    struct token token = lexer_peek(lexer);
    lexer->current.type = TOKEN_UNSET;
    lexer->current.word = NULL;
    return token;
}
