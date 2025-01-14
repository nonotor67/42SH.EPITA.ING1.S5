#include "execution.h"

#include <malloc.h>

#include "utils/utils.h"

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
};

int execute_node(struct ast *node)
{
    static const size_t table_size =
        sizeof(execute_table) / sizeof(execute_table[0]);

    if (node->values)
    {
        size_t values_size = 0;
        while (node->values[values_size])
            values_size++;
        node->expanded_values = malloc((values_size + 1) * sizeof(char *));
        for (size_t i = 0; i < values_size; i++)
            node->expanded_values[i] = word_eval(node->values[i], NULL, NULL);
        node->expanded_values[values_size] = NULL;
    }

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
