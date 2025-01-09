#include "execution.h"

int execute_condition(struct ast *ast)
{
    int left = execute_node(ast->left);
    if (left == 0)
    {
        return execute_node(ast->middle);
    }
    else
    {
        if (ast->right == NULL)
            return left;
        return execute_node(ast->right);
    }
}
