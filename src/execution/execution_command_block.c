#include "execution_command_block.h"

int execute_command_block(struct ast *ast)
{
    return execute_node(ast->left);
}
