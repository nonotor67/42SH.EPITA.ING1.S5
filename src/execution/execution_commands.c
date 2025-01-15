#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

#include "execution.h"
#include "utils/utils.h"

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

static int is_assignment(const char *value)
{
    if (value == NULL)
        return 0;
    size_t i = 0;
    while (isalnum(value[i]) || value[i] == '_')
        i++;
    return value[i] == '=';
}

int dispatch_command(struct ast *ast)
{
    int skipped_assignments = 0;
    while (ast->values[skipped_assignments]
           && is_assignment(ast->values[skipped_assignments]->value.data))
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
        return 0;
    }
    char **real_argv = ast->expanded_values + skipped_assignments;
    if (strcmp(ast->expanded_values[0], "echo") == 0)
        return exec_echo(ast->size, ast->expanded_values);
    if (strcmp(ast->expanded_values[0], "true") == 0)
        return exec_true(ast->size, ast->expanded_values);
    if (strcmp(ast->expanded_values[0], "false") == 0)
        return exec_false(ast->size, ast->expanded_values);
    return run_command(real_argv);
}

int execute_command(struct ast *ast)
{
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
