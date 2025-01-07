#include "token.h"

static void sec_token_print(struct token token)
{
    switch (token.type)
    {
    case TOKEN_QUOTE:
        printf("QUOTE");
        break;
    case TOKEN_DOUBLE_QUOTE:
        printf("DOUBLE_QUOTE");
        break;
    case TOKEN_COMMENT:
        printf("COMMENT");
        break;
    case TOKEN_WORD:
        printf("WORD(%s)", token.value);
        break;
    default:
        printf("UNKNOWN");
        break;
    }
}

void token_print(struct token token)
{
    switch (token.type)
    {
    case TOKEN_EOF:
        printf("EOF");
        break;
    case TOKEN_EOL:
        printf("EOL");
        break;
    case TOKEN_IF:
        printf("IF");
        break;
    case TOKEN_THEN:
        printf("THEN");
        break;
    case TOKEN_ELSE:
        printf("ELSE");
        break;
    case TOKEN_ELIF:
        printf("ELIF");
        break;
    case TOKEN_FI:
        printf("FI");
        break;
    case TOKEN_SEMICOLON:
        printf("SEMICOLON");
        break;
    default:
        sec_token_print(token);
        break;
    }
}
