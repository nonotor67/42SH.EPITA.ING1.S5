#ifdef LEXER_H
#define LEXER_H

#include "token.h"

struct lexer
{
    const char *data;
    size_t pos;
    struct token current;
};

struct lexer *lexer_new(const char *data);

void lexer_free(struct lexer *lexer);

struct lexer *lexer_next(struct lexer *lexer);

struct lexer *lexer_peek(struct lexer *lexer);

struct lexer *lexer_pop(struct lexer *lexer);

void lexer_refill(struct lexer *lexer);

#endif // LEXER_H
