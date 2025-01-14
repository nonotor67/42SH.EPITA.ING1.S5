#include "execution.h"
#include "ast/ast.h"

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
