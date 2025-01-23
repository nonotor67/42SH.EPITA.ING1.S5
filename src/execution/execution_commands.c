#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utils/hashMapFunc.h>
#include <utils/utils.h>

#include "execution.h"

/*
@description:
    Execute all non-builtin commands
@params:
    ast: AST node of type SIMPLE_COMMAND
@return:
    exit status of the command
 */
static int run_command(char **argv)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(argv[0], argv);
        exit(127);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        int exit_status = WEXITSTATUS(status);
        if (exit_status == 127)
        {
            fprintf(stderr, "Command not found: %s\n", argv[0]);
            return 127;
        }
        return WEXITSTATUS(status);
    }
    return 0;
}

static int is_assignment(struct word *word)
{
    if (word == NULL || !word->valid_assignment)
        return 0;
    char *value = word->value.data;
    size_t i = 0;
    while (isalnum(value[i]) || value[i] == '_')
        i++;
    return value[i] == '=';
}

int dispatch_command(struct ast *ast)
{
    int status = 0;
    int skipped_assignments = 0;
    while (ast->values[skipped_assignments]
           && is_assignment(ast->values[skipped_assignments]))
        skipped_assignments++;
    if (skipped_assignments == ast->size)
    {
        // only assignments
        for (int i = 0; i < ast->size; i++)
        {
            // position of the equal sign
            char *var = ast->values[i]->value.data;
            char *equal_sign = var;
            while (*equal_sign != '=')
                equal_sign++;
            *equal_sign = '\0'; // replace the equal sign with a null byte
            insertVariable(var, equal_sign + 1);
            *equal_sign = '='; // put the equal sign back
        }
        char *buf = xmalloc(256);
        my_itoa(status, buf);
        insertVariable("?", buf);
        free(buf);

        return status;
    }
    char **real_argv = ast->expanded_values + skipped_assignments;
    int argc = 0;
    while (real_argv[argc])
        argc++;
    if (getFunction(ast->expanded_values[0]))
        return run_function(argc, ast->expanded_values);
    if (strcmp(ast->expanded_values[0], "echo") == 0)
        status = exec_echo(argc, ast->expanded_values);
    else if (strcmp(ast->expanded_values[0], "true") == 0)
        status = exec_true(argc, ast->expanded_values);
    else if (strcmp(ast->expanded_values[0], "false") == 0)
        status = exec_false(argc, ast->expanded_values);
    else if (strcmp(ast->expanded_values[0], "continue") == 0)
        exec_continue(argc, ast->expanded_values);
    else if (strcmp(ast->expanded_values[0], "break") == 0)
        exec_break(argc, ast->expanded_values);
    else if (strcmp(ast->expanded_values[0], ".") == 0)
        status = exec_dot(argc, ast->expanded_values);
    else if (strcmp(ast->expanded_values[0], "cd") == 0)
        status = exec_cd(argc, ast->expanded_values);
    else if (strcmp(ast->expanded_values[0], "exit") == 0)
        exec_exit(argc, ast->expanded_values);
    else if (strcmp(ast->expanded_values[0], "export") == 0)
        status = exec_export(argc, ast->expanded_values);
    else if (strcmp(ast->expanded_values[0], "unset") == 0)
        status = exec_unset(argc, ast->expanded_values);
    else
        status = run_command(real_argv);

    char *buf = xmalloc(256);
    my_itoa(status, buf);
    insertVariable("?", buf);
    free(buf);

    return status;
}

int execute_command(struct ast *ast)
{
    // Check if the command must be executed
    // If the loop status is set to CONTINUE_LOOP or BREAK_LOOP
    if (loop_status && loop_status->status != 0)
        return 0;
    if (ast->redir)
        return exec_redir(ast);
    return dispatch_command(ast);
}

int execute_command_list(struct ast *ast)
{
    int status = 0;
    if (ast->left != NULL)
        status = execute_node(ast->left);
    if (ast->right != NULL)
        status = execute_command_list(ast->right);
    return status;
}
