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

static int eat(struct lexer *lexer, enum token_type type)
{
    struct token token = lexer_peek(lexer);
    if (token.type == type)
    {
        lexer_pop(lexer);
        return 1;
    }
    return 0;
}

struct ast *simple_command(struct parser *parser, struct lexer *lexer)
{
    struct token token = lexer_peek(lexer);
    if (token.type == TOKEN_WORD)
    {
        struct ast *node = ast_new(SIMPLE_COMMAND);
        int buffer_size = 16;
        node->size = 0;
        node->values = malloc(sizeof(char *) * buffer_size);
        CHECK_ALLOCATION(node->values);
        while (token.type == TOKEN_WORD)
        {
            lexer_pop(lexer);
            if (node->size >= buffer_size - 1)
            {
                buffer_size *= 2;
                node->values =
                    realloc(node->values, sizeof(char *) * buffer_size);
                CHECK_ALLOCATION(node->values);
            }
            node->values[node->size] = token.value;
            node->size++;
            token = lexer_peek(lexer);
        }
        node->values[node->size] = NULL;
        return node;
    }
    fprintf(stderr, "Error: Expected a word token (simple_command)\n");
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return NULL;
}

/*
command =
    simple_command
    | shell_command  { redirection }
    | funcdec { redirection }
    ;
not in step 1
*/
struct ast *command(struct parser *parser, struct lexer *lexer)
{
    return simple_command(parser, lexer);
}

/*
pipeline = ['!'] command { '|' {'\n'} command } ;
not in step 1
*/
struct ast *pipeline(struct parser *parser, struct lexer *lexer)
{
    return command(parser, lexer);
}

/*
and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
not in step 1
*/
struct ast *and_or(struct parser *parser, struct lexer *lexer)
{
    return pipeline(parser, lexer);
}

/*
list = and_or { ( ';' | '&' ) and_or } [ ';' | '&' ] ;
For step 1, only works for ';' and not for '&' yet.
*/
struct ast *list(struct parser *parser, struct lexer *lexer)
{
    struct ast *root = ast_new(COMMAND_LIST);
    root->left = and_or(parser, lexer);
    CHECK_STATUS(parser, root, "Error after parsing and_or (list)\n");

    struct token token = lexer_peek(lexer);
    struct ast *current = root;
    while (token.type == TOKEN_SEMICOLON) // || AMPERSAND later
    {
        lexer_pop(lexer);
        enum token_type next = lexer_peek(lexer).type;

        if (next == TOKEN_EOF || next == TOKEN_EOL)
            // corresponding to the [ ';' | '&' ] part of the rule
            break;

        current->right = ast_new(COMMAND_LIST);
        current = current->right;
        current->left = and_or(parser, lexer);
        CHECK_STATUS(parser, root, "Error after parsing and_or (list)\n");

        token = lexer_peek(lexer);
    }

    if (current == root)
    {
        // if it is a single command, we don't need a COMMAND_LIST node
        root = root->left;
        free(current);
    }

    return root;
}

/*
input =
    list '\n'
    | list EOF
    | '\n'
    | EOF
    ;
*/
struct ast *input(struct parser *parser, struct lexer *lexer)
{
    struct token tok = lexer_peek(lexer);
    parser->status = tok.type == TOKEN_EOF ? PARSER_EOF : PARSER_OK;

    if (tok.type == TOKEN_EOL || tok.type == TOKEN_EOF)
        return NULL; // empty command

    struct ast *ast = list(parser, lexer);
    CHECK_STATUS(parser, ast, "Error after parsing list (input)\n");

    if (!eat(lexer, TOKEN_EOL) && !eat(lexer, TOKEN_EOF))
        parser->status = PARSER_UNEXPECTED_TOKEN;

    //CHECK_STATUS(parser, ast, "Error after parsing EOL or EOF (input)\n");
    return ast;
}

struct ast *parse(struct parser *parser, struct lexer *lexer)
{
    return input(parser, lexer);
}
