#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"

struct ast *ast_new(enum ast_type type)
{
    struct ast *ast = malloc(sizeof(struct ast));
    if (!ast)
    {
        fprintf(stderr, "Failed allocating memory for AST node\n");
        return NULL;
    }

    ast->type = type;
    ast->left = NULL;
    ast->right = NULL;
    ast->middle = NULL;
    ast->size = 0;
    ast->values = NULL;
    ast->expanded_values = NULL;
    ast->expanded_redir = NULL;
    ast->redir = NULL;
    ast->redir_size = 0;

    return ast;
}

void ast_free(struct ast *ast)
{
    if (ast == NULL)
        return;

    if (ast->left)
        ast_free(ast->left);
    if (ast->right)
        ast_free(ast->right);
    if (ast->middle)
        ast_free(ast->middle);

    if (ast->values)
    {
        for (size_t i = 0; ast->values[i]; i++)
            word_free(ast->values[i]);
        free(ast->values);
    }
    if (ast->expanded_values)
    {
        for (size_t i = 0; ast->expanded_values[i]; i++)
            free(ast->expanded_values[i]);
        free(ast->expanded_values);
    }
    if (ast->redir)
    {
        for (size_t i = 0; ast->redir[i]; i++)
            word_free(ast->redir[i]);
        free(ast->redir);
    }

    free(ast);
}
