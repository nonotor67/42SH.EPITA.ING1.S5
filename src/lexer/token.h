#ifndef TOKEN_H
#define TOKEN_H

#include <unistd.h>

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
    TOKEN_WORD,
};

struct token
{
    enum token_type type; // The kind of token
    char *value; // If the token is a word
};

#endif /* !TOKEN_H */
