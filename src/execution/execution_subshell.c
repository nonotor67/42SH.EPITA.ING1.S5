#include "execution_subshell.h"

#include <stdio.h>

int execute_subshell(struct ast *ast)
{
    // Save the current variables
    struct HashMapVar saved_variables = copy_hash_map_var();
    struct HashMapFunc saved_functions = copy_hash_map_func();
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
    free_hash_map_func();
    setFunctionMap(saved_functions);

    // Restore the current directory
    insertVariable("PWD", pwd);
    insertVariable("OLDPWD", oldpwd);
    if (chdir(pwd) == -1)
    {
        fprintf(stderr, "Error: Could not change directory to %s\n", pwd);
        return 1;
    }
    free(pwd);
    free(oldpwd);

    return status;
}
