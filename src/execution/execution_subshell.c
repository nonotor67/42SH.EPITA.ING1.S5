#include "execution_subshell.h"

int execute_subshell(struct ast *ast)
{
    // Save the current variables
    struct HashMapVar saved_variables = copy_hash_map_var();
    // Save the current directory
    // Copy PWD and OLDPWD
    char *pwd = malloc(strlen(getVariable("PWD").value) + 1);
    strcpy(pwd, getVariable("PWD").value);
    char *oldpwd = malloc(strlen(getVariable("OLDPWD").value) + 1);
    strcpy(oldpwd, getVariable("OLDPWD").value);

    int status = execute_node(ast->left);

    // Restore the saved variables
    free_hash_map_var();
    setVariableMap(saved_variables);

    // Restore the current directory
    insertVariable("PWD", pwd);
    insertVariable("OLDPWD", oldpwd);
    chdir(pwd);
    free(pwd);
    free(oldpwd);

    return status;
}
