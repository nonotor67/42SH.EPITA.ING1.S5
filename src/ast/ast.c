#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    ast->value = NULL;
    ast->values = NULL;

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

    // No need to free the value as it is values[0]
    if (ast->values)
    {
        for (size_t i = 0; ast->values[i]; i++)
            free(ast->values[i]);
        free(ast->values);
    }

    free(ast);
}
