#include "execution_subshell.h"

int execute_subshell(struct ast *ast)
{
    struct HashMapVar saved_variables = copy_hash_map_var();

    int status = execute_node(ast->left);

    free_hash_map_var();
    setVariableMap(saved_variables);

    return status;
}
