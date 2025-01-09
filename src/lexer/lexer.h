#ifndef LEXER_H
#define LEXER_H

#include "../io/io.h"
#include "token.h"

enum lexing_mode
{
    LEXING_NORMAL,
    LEXING_QUOTED,
    LEXING_DOUBLE_QUOTED,
    LEXING_COMMENT,
    // Add more modes if needed
};

struct lexer
{
    struct reader *reader;
    int current_char;
    struct token current;
    int escape_next;
    enum lexing_mode mode;
};

// Create a new lexer based on a reader
struct lexer *lexer_new(struct reader *reader);

// Free the lexer
void lexer_free(struct lexer *lexer);

// INTERNAL, do not use if not aware,
// Use peek or pop instead
// Get the next token
struct token lexer_next(struct lexer *lexer);

// Peek the next token
// Does not consume the token
struct token lexer_peek(struct lexer *lexer);

// Pop the next token
// Consumes the token
struct token lexer_pop(struct lexer *lexer);

// Set the lexer mode
void lexer_set_mode(struct lexer *lexer, enum lexing_mode mode);

#endif // LEXER_H
