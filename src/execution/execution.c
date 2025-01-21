#include "execution.h"

#include <stdio.h>

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
    { UNTIL_LOOP, execution_until },
    { FOR_LOOP, execution_for },
       {SUBSHELL, execute_subshell}
};

/*
@brief Expands the values of a source word array and stores them in a target
array.
@param target Pointer to a char** array, which will be allocated and filled with
the evaluated values.
@param source Array of pointers to 'word' structures, which contains the
elements to be evaluated.
@return 0 if the expansion was successful, 1 otherwise.
 */
static int expand_values(char ***target, struct word **source)
{
    size_t size = 0;
    while (source[size])
        size++;
    if (*target)
    {
        for (size_t i = 0; i < size; i++)
            free((*target)[i]);
        free(*target);
    }
    *target = xmalloc((size + 1) * sizeof(char *));
    size_t j = 0;
    int failed = 0;
    for (size_t i = 0; i < size; i++)
    {
        failed = failed || !is_word_valid(source[i]);
        char *val = word_eval(source[i]);
        if (val && (*val != '\0' || source[i - 1]->has_escaped))
            (*target)[j++] = val;
        else
            free(val);
    }
    (*target)[j] = NULL;
    return failed;
}

#define EXPAND_VALUES(vect, target)                                            \
    if (vect && expand_values(&target, vect))                                  \
    {                                                                          \
        fprintf(stderr, "Failed to expand values\n");                          \
        return 1;                                                              \
    }

int execute_node(struct ast *node)
{
    static const size_t table_size =
        sizeof(execute_table) / sizeof(execute_table[0]);

    // returns 1 when expansion fails
    EXPAND_VALUES(node->values, node->expanded_values)
    EXPAND_VALUES(node->redir, node->expanded_redir)

    if (node->redir)
        return exec_redir(node);

    for (size_t i = 0; i < table_size; i++)
        if (execute_table[i].type == node->type)
            return execute_table[i].func(node);

    return 1;
}

int execution(struct ast *ast)
{
    if (ast == NULL)
        return 0;
    return execute_node(ast);
}
