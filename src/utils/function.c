#include "function.h"

#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "utils.h"

struct ast *ast_copy(struct ast *ast)
{
    if (ast == NULL)
    {
        return NULL;
    }

    struct ast *copy = xmalloc(sizeof(struct ast));
    copy->type = ast->type;
    copy->left = ast_copy(ast->left);
    copy->middle = ast_copy(ast->middle);
    copy->right = ast_copy(ast->right);
    copy->size = ast->size;
    copy->redir_size = ast->redir_size;
    copy->expanded_values = NULL;
    copy->expanded_redir = NULL;

    // Copy values
    size_t var_size = 0;
    while (ast->values && ast->values[var_size])
        var_size++;
    copy->values = xmalloc((var_size + 1) * sizeof(struct word *));
    for (size_t i = 0; i < var_size; i++)
        copy->values[i] = word_copy(ast->values[i]);
    copy->values[var_size] = NULL;

    // Copy redirections
    size_t redir_size = 0;
    while (ast->redir && ast->redir[redir_size])
        redir_size++;
    copy->redir = xmalloc((redir_size + 1) * sizeof(struct word *));
    for (size_t i = 0; i < redir_size; i++)
        copy->redir[i] = word_copy(ast->redir[i]);
    copy->redir[redir_size] = NULL;

    return copy;
}

struct function *create_function(char *name, struct ast *value)
{
    struct function *func = xmalloc(sizeof(struct function));
    func->name = strcpy(xmalloc(strlen(name) + 1), name);
    func->value = ast_copy(value);
    func->next = NULL;

    return func;
}

void free_function(struct function *func)
{
    free(func->name);
    ast_free(func->value);
    free(func);
}
