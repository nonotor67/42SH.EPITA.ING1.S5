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
static int run_command(struct ast *ast)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(ast->values[0], ast->values);
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    return 0;
}

int execute_command(struct ast *ast)
{
    if (strcmp(ast->values[0], "echo") == 0)
        return exec_echo(ast->size, ast->values);
    if (strcmp(ast->values[0], "true") == 0)
        return exec_true(ast->size, ast->values);
    if (strcmp(ast->values[0], "false") == 0)
        return exec_false(ast->size, ast->values);
    return run_command(ast);
}

int execute_command_list(struct ast *ast)
{
    int status = 0;
    if (ast->left != NULL)
        status = execute_command(ast->left);
    if (ast->right != NULL)
        status = execute_command_list(ast->right);
    return status;
}
