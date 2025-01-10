#ifndef PARSER_STRUCT_H
#define PARSER_STRUCT_H

#include "lexer/lexer.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
    PARSER_EOF,
};

struct parser
{
    enum parser_status status;
    struct lexer *lexer;
};

#endif /* !PARSER_STRUCT_H */
