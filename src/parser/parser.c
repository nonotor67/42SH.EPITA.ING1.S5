#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "lexer/lexer.h"

struct ast *simple_command(enum parser_status *status, struct lexer *lexer)
{
    struct token token = lexer_pop(lexer);
    if (token.type == TOKEN_WORD)
    {
        struct ast *node = ast_new(SIMPLE_COMMAND);
        int buffer_size = 16;
        node->values = malloc(sizeof(char *) * buffer_size);
        if (!node->values)
        {
            fprintf(stderr, "Failed allocating memory for values\n");
            *status = PARSER_UNEXPECTED_TOKEN;
            return NULL;
        }
        while (token.type == TOKEN_WORD)
        {
            if (node->size >= buffer_size - 1)
            {
                buffer_size *= 2;
                node->values =
                    realloc(node->values, sizeof(char *) * buffer_size);
                if (!node->values)
                {
                    fprintf(stderr, "Failed reallocating memory for values\n");
                    *status = PARSER_UNEXPECTED_TOKEN;
                    return NULL;
                }
            }
            node->values[node->size] = token.value;
            node->size++;
            token = lexer_pop(lexer);
        }
        node->values[node->size] = NULL;
        return node;
    }
    *status = PARSER_UNEXPECTED_TOKEN;
    return NULL;
}

struct ast *command(enum parser_status *status, struct lexer *lexer)
{
    return simple_command(status, lexer);
}

struct ast *pipeline(enum parser_status *status, struct lexer *lexer)
{
    return command(status, lexer);
}

struct ast *and_or(enum parser_status *status, struct lexer *lexer)
{
    return pipeline(status, lexer);
}

struct ast *list(enum parser_status *status, struct lexer *lexer)
{
    return and_or(status, lexer);
}

struct ast *input(enum parser_status *status, struct lexer *lexer);
struct ast *parse(enum parser_status *status, struct lexer *lexer)
{
    return input(status, lexer);
}