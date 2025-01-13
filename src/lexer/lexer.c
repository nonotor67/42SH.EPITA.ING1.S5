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
    static const char reserved[] = " \t\n;#|&<>'\"";
    for (size_t i = 0; i < sizeof(reserved) - 1; i++)
        if (c == reserved[i])
            return 0;
    return 1;
}

#define BLIND_PUSH_WHEN(cond)                                                  \
    if (cond)                                                                  \
    {                                                                          \
        string_push(word, last_char(lexer));                                   \
        next_char(lexer);                                                      \
        lexer->escape_next = 0;                                                \
        continue;                                                              \
    }

// Returns true if it is a redirection and adds the redirection to the word
static int lexer_try_redir(struct lexer *lexer, struct string *word)
{
    char c = (char)last_char(lexer);
    if (c != '>' && c != '<')
        return 0;
    // check if word is a number
    for (size_t i = 0; i < word->length; i++)
        if (!isdigit(word->data[i]))
            return 0;

    string_push(word, c);
    next_char(lexer);

    // check if the next character completes the redirection
    char c2 = (char)last_char(lexer);
    char str[] = { c, c2, '\0' };
    if (strcmp(str, ">>") == 0 || strcmp(str, "<<") == 0
        || strcmp(str, "<>") == 0 || strcmp(str, ">&") == 0
        || strcmp(str, "<&") == 0 || strcmp(str, ">|") == 0)
    {
        string_push(word, c2);
        next_char(lexer);
    }

    return 1;
}

// Create a word token or a keyword token
static struct token lexer_next_handle_word(struct lexer *lexer)
{
    struct string *word = string_new();
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
        // TODO: add expanding logic
        BLIND_PUSH_WHEN(lexer->mode == LEXING_DOUBLE_QUOTED);

        if (c == '\\' && lexer->mode == LEXING_NORMAL)
        {
            lexer->escape_next = 1;
            next_char(lexer);
            continue;
        }

        BLIND_PUSH_WHEN(1);
    }

    struct token token;
    token.type = TOKEN_WORD;

    if (!has_escaped && lexer_try_redir(lexer, word))
        token.type = TOKEN_REDIR;

    token.value = word->data;
    free(word); // only the string struct is freed, not the data
    return token;
}

// Switch the token based on the character
static struct token lexer_switch(struct lexer *lexer)
{
    struct token token;
    token.value = NULL;
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
    lexer->current.value = NULL;
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
        token.value = NULL;
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
    lexer->current.value = NULL;
    return token;
}
