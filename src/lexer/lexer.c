#include "lexer.h"

#include <io/io.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer_internal.h"

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
    lexer->context = LEXING_COMMAND;

    return lexer;
}

void lexer_free(struct lexer *lexer)
{
    free(lexer);
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

void lexer_context_begin(struct lexer *lexer)
{
    lexer->context = LEXING_COMMAND;
}
