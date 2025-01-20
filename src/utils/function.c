#include "function.h"

#include <string.h>
#include "utils.h"
#include <stdlib.h>

#include "ast/ast.h"

static struct ast *ast_copy(struct ast *ast)
{
       if (ast == NULL)
       {
              return NULL;
       }

       struct ast *copy = xmalloc(sizeof(struct ast));
       copy->type = ast->type;
       copy->values = ast->values;
       copy->left = ast_copy(ast->left);
       copy->right = ast_copy(ast->right);

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
