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
    { OR, execution_or },
    { AND, execution_and },
    { PIPELINE, execute_pipeline },
    { NEGATION, execute_negation },
    { WHILE_LOOP, execution_while },
    { UNTIL_LOOP, execution_until }
};

/*
@brief Expands the values of a source word array and stores them in a target
array.
@param target Pointer to a char** array, which will be allocated and filled with
the evaluated values.
@param source Array of pointers to 'word' structures, which contains the
elements to be evaluated.
 */
static void expand_values(char ***target, struct word **source)
{
    size_t size = 0;
    while (source[size])
        size++;
    *target = malloc((size + 1) * sizeof(char *));
    for (size_t i = 0; i < size; i++)
        (*target)[i] = word_eval(source[i]);
    (*target)[size] = NULL;
}

int execute_node(struct ast *node)
{
    static const size_t table_size =
        sizeof(execute_table) / sizeof(execute_table[0]);

    if (node->values)
        expand_values(&node->expanded_values, node->values);

    if (node->redir)
        expand_values(&node->expanded_redir, node->redir);

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
