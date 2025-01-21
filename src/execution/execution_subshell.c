#include "execution_subshell.h"

int execute_subshell(struct ast *ast)
{
    // TODO: Add for function
    // Save the variables
    struct HashMapVar saved_variables = copy_hash_map_var();
    int status = execute_node(ast->left);
    // Restore the variables
    restore_hash_map_var(saved_variables);
    return status;
}
