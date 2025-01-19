#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Contains the creation and top layer of the parser
--> list
--> input
*/

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

/*
list = and_or { ( ';' | '&' ) and_or } [ ';' | '&' ] ;
For step 1, only works for ';' and not for '&' yet.
*/
struct ast *list(struct parser *parser)
{
    struct ast *root = ast_new(COMMAND_LIST);
    root->left = and_or(parser);
    struct token tok;
    CHECK_STATUS(parser, root, "Error after parsing and_or (list)\n");

    struct token token = lexer_peek(parser->lexer);
    struct ast *current = root;
    while (token.type == TOKEN_SEMICOLON) // || AMPERSAND later
    {
        lexer_pop(parser->lexer);
        enum token_type next = lexer_peek(parser->lexer).type;

        if (next == TOKEN_EOF || next == TOKEN_EOL)
            // corresponding to the [ ';' | '&' ] part of the rule
            break;

        current->right = ast_new(COMMAND_LIST);
        current = current->right;
        current->left = and_or(parser);
        CHECK_STATUS(parser, root, "Error after parsing and_or (list)\n");

        token = lexer_peek(parser->lexer);
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
struct ast *input(struct parser *parser)
{
    struct token tok = lexer_peek(parser->lexer);
    parser->status = tok.type == TOKEN_EOF ? PARSER_EOF : PARSER_OK;

    if (tok.type == TOKEN_EOL)
    {
        lexer_pop(parser->lexer);
        return NULL; // empty command
    }
    if (tok.type == TOKEN_EOF)
        return NULL; // empty command

    struct ast *ast = list(parser);
    CHECK_STATUS(parser, ast, "Error after parsing list (input)\n");

    if (!eat(parser->lexer, TOKEN_EOL) && !eat(parser->lexer, TOKEN_EOF))
        parser->status = PARSER_UNEXPECTED_TOKEN;

    CHECK_STATUS(parser, ast, "Error after parsing EOL or EOF (input)\n");
    return ast;
}

struct ast *parse(struct parser *parser)
{
    struct ast *ast = input(parser);
    return ast;
}
