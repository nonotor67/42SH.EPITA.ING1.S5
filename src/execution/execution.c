#include "execution.h"

int execute_node(struct ast *node)
{
    switch (node->type)
    {
    case SIMPLE_COMMAND:
        return execute_command(node);
    case COMMAND_LIST:
        return execute_command_list(node);
    case CONDITIONS:
        return execute_condition(node);
    default:
        return 1;
    }
}

int execution(struct ast *ast)
{
    if (ast == NULL)
        return 0;
    return execute_node(ast);
}