#ifndef LEXER_INTERNAL_H
#define LEXER_INTERNAL_H

#include "lexer.h"

// INTERNAL, do not use if not aware,
// Use peek or pop instead
// Get the next token
struct token lexer_next(struct lexer *lexer);

#endif // !LEXER_INTERNAL_H
