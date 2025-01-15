#include "ast/ast.h"
#include "execution.h"
#include "utils/utils.h"

int execution_while(struct ast *ast)
{
    int status = 0;
    while (execute_node(ast->left) == 0)
    {
        status = execute_node(ast->right);
    }
    return status;
}

int execution_until(struct ast *ast)
{
    int status = 0;
    while (execute_node(ast->left) != 0)
    {
        status = execute_node(ast->right);
    }
    return status;
}

int execution_for(struct ast *ast)
{
    int status = 0;

    int i = 0;
    while (ast->middle->values[i])
    {
        insertVariable(ast->left->values[0]->value.data, ast->middle->values[i]->value.data);
        status = execute_node(ast->right);
        i++;
    }

    return status;
}
