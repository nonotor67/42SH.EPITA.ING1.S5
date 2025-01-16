#include "parser.h"

/*
Contains all the functions to parse the operator part of the grammar
--> Pipelines
--> And/Or
*/

/*
pipeline = ['!'] command { '|' {'\n'} command } ;
not in step 1
*/
struct ast *pipeline(struct parser *parser)
{
    int negate = 0;
    struct token tok = lexer_peek(parser->lexer);
    if (tok.type == TOKEN_NEGATION)
    {
        lexer_pop(parser->lexer);
        negate = 1;
    }
    struct ast *root = ast_new(PIPELINE);
    root->left = command(parser);
    CHECK_STATUS(parser, root, "Error after parsing command (pipeline[0])\n");
    tok = lexer_peek(parser->lexer);
    struct ast *current = root;
    while (tok.type == TOKEN_PIPE)
    {
        lexer_pop(parser->lexer);
        skip_eol(parser->lexer);
        current->right = ast_new(PIPELINE);
        current = current->right;
        current->left = command(parser);
        CHECK_STATUS(parser, root,
                     "Error after parsing command (pipeline[1])\n");
        tok = lexer_peek(parser->lexer);
    }
    if (current == root)
    {
        root = root->left;
        free(current);
    }
    if (negate == 1)
    {
        struct ast *new_ast = ast_new(NEGATION);
        new_ast->left = root;
        return new_ast;
    }
    return root;
}

/*
and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
*/
struct ast *and_or(struct parser *parser)
{
    struct token tok;
    struct ast *ast = pipeline(parser);
    CHECK_STATUS(parser, ast, "Error after parsing pipeline (and_or)\n");
    tok = lexer_peek(parser->lexer);
    while (tok.type == TOKEN_AND || tok.type == TOKEN_OR)
    {
        struct ast *new_ast = ast_new(tok.type == TOKEN_AND ? AND : OR);
        new_ast->left = ast;
        lexer_pop(parser->lexer);
        skip_eol(parser->lexer);
        new_ast->right = pipeline(parser);
        CHECK_STATUS(parser, new_ast,
                     "Error after parsing pipeline (and_or)\n");
        ast = new_ast;
        tok = lexer_peek(parser->lexer);
    }
    return ast;
}
