#include "execution_or_and.h"

int execution_or(struct ast *ast)
{
    // Lazy evaluation
    int left = execute_node(ast->left);
    if (left == 0)
        return left;
    return execute_node(ast->right);
}

int execution_and(struct ast *ast)
{
    // Lazy evaluation
    int left = execute_node(ast->left);
    if (left != 0)
        return left;
    return execute_node(ast->right);
}
