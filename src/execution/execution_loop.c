#include "ast/ast.h"
#include "execution.h"
#include "utils/utils.h"

struct loop_status *loop_status = NULL;

static void init_loop_status(void)
{
    loop_status = malloc(sizeof(struct loop_status));
    loop_status->status = 0;
    loop_status->occurence = 0;
}

static void free_loop_status(struct loop_status *status_to_free)
{
    if (status_to_free)
    {
        free(status_to_free);
    }
}

int execution_while(struct ast *ast)
{
    // Store the loop status to restore it after the loop
    struct loop_status *stored_loop_status = loop_status;
    init_loop_status();

    loop_status->status = 0;
    loop_status->occurence = 0;

    int status = 0;
    while (execute_node(ast->left) == 0)
    {
        if (loop_status->status == BREAK_LOOP)
            break;
        if (loop_status->status == CONTINUE_LOOP)
        {
            if (loop_status->occurence >= 1)
            {
                loop_status->occurence--;
                continue;
            }
            else
                loop_status->status = 0;
        }
        status = execute_node(ast->right);
    }

    struct loop_status *current_loop_status = loop_status;
    loop_status = stored_loop_status;
    free_loop_status(current_loop_status);
    return status;
}

int execution_until(struct ast *ast)
{
    // Store the loop status to restore it after the loop
    struct loop_status *stored_loop_status = loop_status;
    init_loop_status();
    int status = 0;
    while (execute_node(ast->left) != 0)
    {
        if (loop_status->status == BREAK_LOOP)
            break;
        if (loop_status->status == CONTINUE_LOOP)
        {
            if (loop_status->occurence >= 1)
            {
                loop_status->occurence--;
                continue;
            }
            else
                loop_status->status = 0;
        }
        status = execute_node(ast->right);
    }
    struct loop_status *current_loop_status = loop_status;
    loop_status = stored_loop_status;
    free_loop_status(current_loop_status);
    return status;
}

int execution_for(struct ast *ast)
{
    // Store the loop status to restore it after the loop
    struct loop_status *stored_loop_status = loop_status;
    if (!loop_status)
        init_loop_status();
    else
        loop_status = malloc(sizeof(struct loop_status));
    int status = 0;
    int i = 0;
    while (ast->middle && ast->middle->values[i])
    {
        if (loop_status->status == BREAK_LOOP)
            break;
        if (loop_status->status == CONTINUE_LOOP)
        {
            if (loop_status->occurence >= 1)
            {
                loop_status->occurence--;
                continue;
            }
            else
                loop_status->status = 0;
        }
        insertVariable(ast->left->values[0]->value.data,
                       ast->middle->values[i]->value.data);
        status = execute_node(ast->right);
        i++;
    }
    struct loop_status *current_loop_status = loop_status;
    loop_status = stored_loop_status;
    free_loop_status(current_loop_status);
    return status;
}
