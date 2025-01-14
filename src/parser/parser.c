#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "lexer/lexer.h"

#define CHECK_STATUS(parser, ast, msg)                                         \
    if (parser->status != PARSER_OK)                                           \
    {                                                                          \
        if (ast)                                                               \
            free(ast);                                                         \
        fprintf(stderr, msg);                                                  \
        return NULL;                                                           \
    }

#define CHECK_ALLOCATION(ptr)                                                  \
    if (!ptr)                                                                  \
    {                                                                          \
        fprintf(stderr, "Failed allocating memory\n");                         \
        exit(1);                                                               \
    }

#define EXPECT_TOKEN(lexer, type, msg)                                         \
    if (!eat(lexer, type))                                                     \
    {                                                                          \
        fprintf(stderr, msg);                                                  \
        parser->status = PARSER_UNEXPECTED_TOKEN;                              \
        return NULL;                                                           \
    }

#define EXPECT_WORD(lexer, word, msg)                                          \
    if (!eat_word(lexer, word))                                                \
    {                                                                          \
        fprintf(stderr, msg);                                                  \
        parser->status = PARSER_UNEXPECTED_TOKEN;                              \
        return NULL;                                                           \
    }

struct parser *parser_new(struct lexer *lexer)
{
    struct parser *parser = malloc(sizeof(struct parser));
    CHECK_ALLOCATION(parser);
    parser->status = PARSER_OK;
    parser->lexer = lexer;
    return parser;
}

void parser_free(struct parser *parser)
{
    free(parser);
}

struct ast *simple_command(struct parser *parser)
{
    struct token token = lexer_peek(parser->lexer);
    if (token.type == TOKEN_WORD)
    {
        struct ast *node = ast_new(SIMPLE_COMMAND);
        int buffer_size = 16;
        node->size = 0;
        node->values = malloc(sizeof(char *) * buffer_size);
        CHECK_ALLOCATION(node->values);
        while (token.type == TOKEN_WORD)
        {
            lexer_pop(parser->lexer);
            if (node->size >= buffer_size - 1)
            {
                buffer_size *= 2;
                node->values =
                    realloc(node->values, sizeof(char *) * buffer_size);
                CHECK_ALLOCATION(node->values);
            }
            node->values[node->size] = token.word;
            node->size++;
            token = lexer_peek(parser->lexer);
        }
        node->values[node->size] = NULL;
        return node;
    }
    fprintf(stderr, "Error: Expected a word token (simple_command)\n");
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return NULL;
}

struct ast *parse(struct parser *parser)
{
    return input(parser);
}
