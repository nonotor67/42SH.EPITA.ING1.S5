#ifndef PARSE_STRUCT_H
#define PARSE_STRUCT_H

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
    PARSER_EOF,
};

struct parser
{
    enum parser_status status;
};

#endif // !PARSE_STRUCT_H
