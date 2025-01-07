#ifndef TOKEN_H
#define TOKEN_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

enum token_type
{
    // End of file, can be called to refill the lexer
    TOKEN_EOF = 0,

    TOKEN_EOL,

    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_FI,
    TOKEN_SEMICOLON,
    TOKEN_QUOTE,
    TOKEN_DOUBLE_QUOTE,
    TOKEN_COMMENT,
    TOKEN_WORD,
};

struct token
{
    enum token_type type; // The kind of token
    char *value; // If the token is a word
};

void token_print(struct token token);

#endif /* !TOKEN_H */
