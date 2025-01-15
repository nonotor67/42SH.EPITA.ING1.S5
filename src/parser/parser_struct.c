#include "parser_struct.h"

#include <string.h>

/*
Contains the base of the parser
--> is_closing_word
--> eat
--> eat_word
--> skip_eol
*/

int is_closing_word(struct word *word)
{
    const char *keywords[] = { "then", "fi", "else", "elif", "do", "done" };
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(keywords[i], word->value.data) == 0)
            return 1;
    }
    return 0;
}

int eat(struct lexer *lexer, enum token_type type)
{
    struct token token = lexer_peek(lexer);
    if (token.type == type)
    {
        lexer_pop(lexer);
        return 1;
    }
    return 0;
}

int eat_word(struct lexer *lexer, char *word)
{
    struct token token = lexer_peek(lexer);
    if (token.type == TOKEN_WORD && token.word
        && strcmp(token.word->value.data, word) == 0)
    {
        word_free(token.word);
        lexer_pop(lexer);
        return 1;
    }
    return 0;
}

void skip_eol(struct lexer *lexer)
{
    while (eat(lexer, TOKEN_EOL))
        continue;
}
