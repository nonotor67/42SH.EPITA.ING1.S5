#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include "../io/io.h"

enum lexing_mode
{
    LEXING_COMMAND, // command name or keyword
    LEXING_NORMAL,  // command arguments
    LEXING_STRING,  // command arguments in quotes
    LEXING_COMMENT, // ignore the rest of the line
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
