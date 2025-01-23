#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils/word.h"

enum token_type
{
    // End of file, can be called to refill the lexer
    TOKEN_EOF = 0,

    TOKEN_EOL,

    TOKEN_UNSET,
    TOKEN_UNKNOWN,

    TOKEN_WORD,
    TOKEN_KEYWORD,
    TOKEN_SEMICOLON,
    TOKEN_REDIR,
    TOKEN_PIPE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NEGATION,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
};

struct token
{
    enum token_type type; // The kind of token
    struct word *word; // The value of the token
};

// void token_print(struct token token);

#endif /* !TOKEN_H */
