#include "token.h"

static void sec_token_print(struct token token)
{
    switch (token.type)
    {
    case TOKEN_UNSET:
        printf("UNSET");
        break;
    case TOKEN_WORD:
        printf("WORD(%s)", token.word->value.data);
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
    case TOKEN_SEMICOLON:
        printf("SEMICOLON");
        break;
    default:
        sec_token_print(token);
        break;
    }
}
