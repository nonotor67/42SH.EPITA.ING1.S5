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

#endif // LEXER_H