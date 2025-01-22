#include "execution_function.h"

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
    if (argc > 1)
    {
        for (int i = 0; i < argc - 1; i++)
        {
            insertVariable(function->expanded_values[i + 1], argv[i + 1]);
        }
    }
    return execute_node(function);
}
