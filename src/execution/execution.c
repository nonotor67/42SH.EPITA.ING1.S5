#include "execution.h"

typedef int (*execute_function)(struct ast *);

struct execute_entry
{
    enum ast_type type;
    execute_function func;
};

static const struct execute_entry execute_table[] = {
    { SIMPLE_COMMAND, execute_command },
    { COMMAND_LIST, execute_command_list },
    { CONDITIONS, execute_condition },
    { OR, execution_or},
    {AND, execution_and},
};

int execute_node(struct ast *node)
{
    static const size_t table_size =
        sizeof(execute_table) / sizeof(execute_table[0]);

    for (size_t i = 0; i < table_size; i++)
    {
        if (execute_table[i].type == node->type)
        {
            return execute_table[i].func(node);
        }
    }
    return 1;
}

int execution(struct ast *ast)
{
    if (ast == NULL)
        return 0;
    return execute_node(ast);
}
