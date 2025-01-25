#ifndef LEXER_H
#define LEXER_H

#include "io/io.h"
#include "token.h"

#define UNITIALIZED_CHAR (-5)

enum lexing_mode
{
    LEXING_NORMAL,
    LEXING_QUOTED,
    LEXING_DOUBLE_QUOTED,
    LEXING_COMMENT,
    // Add more modes if needed
};

enum lexing_context
{
    LEXING_COMMAND, // beginning of string
    LEXING_ARGUMENT,
};

struct lexer
{
    struct reader *reader;
    int current_char;
    struct token current;
    int escape_next;
    enum lexing_mode mode;
    enum lexing_context context;
};

// Create a new lexer based on a reader
struct lexer *lexer_new(struct reader *reader);

// Free the lexer
void lexer_free(struct lexer *lexer);

// Peek the next token
// Does not consume the token
struct token lexer_peek(struct lexer *lexer);

// Pop the next token
// Consumes the token
struct token lexer_pop(struct lexer *lexer);

// Sets the context of the lexer to command beginning
void lexer_context_begin(struct lexer *lexer);

#endif /* !LEXER_H */
