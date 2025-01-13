#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "execution.h"

/*
@description:
    Execute all non-builtin commands
@params:
    ast: AST node of type SIMPLE_COMMAND
@return:
    exit status of the command
 */
static int run_command(char **values)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(values[0], values);
        exit(127);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        int exit_status = WEXITSTATUS(status);
        if (exit_status == 127)
        {
            fprintf(stderr, "Command not found: %s\n", values[0]);
            return 127;
        }
        return WEXITSTATUS(status);
    }
    return 0;
}

int dispatch_command(int size, char **values)
{
    if (strcmp(values[0], "echo") == 0)
        return exec_echo(size, values);
    if (strcmp(values[0], "true") == 0)
        return exec_true(size, values);
    if (strcmp(values[0], "false") == 0)
        return exec_false(size, values);
    return run_command(values);
}

int execute_command(struct ast *ast)
{
    if (ast->redir)
        return exec_redir(ast->size, ast->values, ast->redir);
    else
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
