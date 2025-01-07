#ifndef LEXER_H
#define LEXER_H

#include "token.h"

struct lexer
{
    const char *data;
    size_t pos;
    struct token current;
};


//Create a new lexer based on a string
struct lexer *lexer_new(const char *data);

//Free the lexer
void lexer_free(struct lexer *lexer);

//INTERNAL, do not use if not aware,
//Use peek or pop instead
//Get the next token
struct token lexer_next(struct lexer *lexer);

//Peek the next token
//Does not consume the token
struct token lexer_peek(struct lexer *lexer);

//Pop the next token
//Consumes the token
struct token lexer_pop(struct lexer *lexer);

//Refill the lexer
//It is called by the parser when the lexer is empty
void lexer_refill(struct lexer *lexer);

//Print the lexer and all the tokens remaining in the lexer
void lexer_print(struct lexer *lexer);

#endif // LEXER_H
 