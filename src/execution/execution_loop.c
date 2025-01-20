#include "ast/ast.h"
#include "execution.h"
#include "utils/utils.h"

struct loop_status *loop_status = NULL;

static void init_loop_status()
{
    loop_status = malloc(sizeof(struct loop_status));
    loop_status->status = 0;
    loop_status->occurence = 0;
}

static void free_loop_status()
{
    if (loop_status)
    {
        free(loop_status);
    }
}

int execution_while(struct ast *ast)
{
    // Store the loop status to restore it after the loop
    // Could be NULL
    struct loop_status *stored_loop_status = loop_status;
    if (!loop_status)
        init_loop_status();
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
    free_loop_status();
    loop_status = stored_loop_status;
    return status;
}

int execution_until(struct ast *ast)
{
    struct loop_status *stored_loop_status = loop_status;
    if (!loop_status)
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
    free_loop_status();
    loop_status = stored_loop_status;
    return status;
}

int execution_for(struct ast *ast)
{
    struct loop_status *stored_loop_status = loop_status;
    if (!loop_status)
        init_loop_status();
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
    free_loop_status();
    loop_status = stored_loop_status;
    return status;
}
