#include "execution_conditions.h"

int execute_condition(struct ast *ast)
{
    if (execute_node(ast->left) == 0)
    {
        return execute_node(ast->middle);
    }
    else
    {
        return execute_node(ast->right);
    }
}
