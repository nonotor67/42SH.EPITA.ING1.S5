#include "execution_function.h"

#include <utils/utils.h>

int executution_function(struct ast *ast)
{
    insertFunction(ast->expanded_values[0], ast->left);
    return 0;
}

int run_function(int argc, char *argv[])
{
    struct ast *function = getFunction(argv[0]);
    if (function == NULL)
        return 0;
    char **saved_variables = xmalloc(sizeof(char *) * argc);
    for (int i = 0; i < argc; i++)
    {
        char *int_tmp = xmalloc(256);
        int_tmp = my_itoa(i, int_tmp);
        if (getVariable(my_itoa(i, int_tmp)).value != NULL)
        {
            saved_variables[i] =
                malloc(strlen(getVariable(my_itoa(i, int_tmp)).value) + 1);
            strcpy(saved_variables[i], getVariable(my_itoa(i, int_tmp)).value);
        }
        else
            saved_variables[i] = NULL;
        insertVariable(my_itoa(i, int_tmp), argv[i]);
        free(int_tmp);
    }
    int status = execute_node(function);
    for (int i = 0; i < argc; i++)
    {
        char *int_tmp = xmalloc(256);
        int_tmp = my_itoa(i, int_tmp);
        if (saved_variables[i] == NULL)
            removeVariable(my_itoa(i, int_tmp));
        else
        {
            insertVariable(my_itoa(i, int_tmp), saved_variables[i]);
            free(saved_variables[i]);
        }
        free(int_tmp);
    }
    free(saved_variables);
    return status;
}
