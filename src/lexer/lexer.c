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
    if (lexer->current_char == UNITIALIZED_CHAR) // only the first time
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
static int lexer_is_alphanum(struct lexer *lexer)
{
    const char accepted[] = "_-,.\\";
    if (isalnum(last_char(lexer)))
        return 1;
    for (size_t i = 0; i < sizeof(accepted) - 1; i++)
        if (last_char(lexer) == accepted[i])
            return 1;
    return 0;
}

// Create a word token or a keyword token
static struct token lexer_next_handle_word(struct lexer *lexer)
{
    struct string *word = string_new();
    while (lexer_is_alphanum(lexer) || lexer->escape_next
           || lexer->mode == LEXING_QUOTED
           || lexer->mode == LEXING_DOUBLE_QUOTED)
    {
        char c = (char)last_char(lexer);
        if (lexer->escape_next)
        {
            // when escaped, append no matter what
            lexer->escape_next = 0;
            string_push(word, c);
            next_char(lexer);
            continue;
        }
        if (lexer->mode == LEXING_QUOTED || lexer->mode == LEXING_DOUBLE_QUOTED)
        {
            const char quote = lexer->mode == LEXING_QUOTED ? '\'' : '"';
            if (c == quote)
            {
                lexer->mode = LEXING_NORMAL;
                next_char(lexer);
                continue;
            }
            string_push(word, c);
            next_char(lexer);
            continue;
        }

        if (c == '\\' && lexer->mode == LEXING_NORMAL)
        {
            lexer->escape_next = 1;
            next_char(lexer);
            continue;
        }

        string_push(word, c);
        next_char(lexer);
    }

    struct token token;
    token.type = TOKEN_WORD;
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
        next_char(lexer);
        return lexer_next_handle_word(lexer);
    case '"':
        lexer->mode = LEXING_DOUBLE_QUOTED;
        next_char(lexer);
        return lexer_next_handle_word(lexer);
    case '#':
        lexer->mode = LEXING_COMMENT;
        next_char(lexer);
        return lexer_next(lexer);
    default:
        token.type = TOKEN_UNKNOWN;
        break;
    }
    next_char(lexer);
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

    if (lexer_is_alphanum(lexer) || lexer->escape_next)
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
