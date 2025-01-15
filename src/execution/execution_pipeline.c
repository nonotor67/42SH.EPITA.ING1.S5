#include "execution_pipeline.h"

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int execute_pipeline(struct ast *ast)
{
    if (ast->right == NULL)
        return execute_node(ast->left);

    int pipefd[2];
    pid_t pid_left, pid_right;
    int status_left = 0;
    int status_right = 0;

    if (pipe(pipefd) == -1)
    {
        return 1;
    }

    pid_left = fork();
    if (pid_left == -1)
    {
        return 1;
    }

    if (pid_left == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        exit(execute_node(ast->left));
    }

    close(pipefd[1]);

    pid_right = fork();
    if (pid_right == -1)
    {
        return 1;
    }

    if (pid_right == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        exit(execute_node(ast->right));
    }

    close(pipefd[0]);

    waitpid(pid_left, &status_left, 0);
    waitpid(pid_right, &status_right, 0);

    if (WIFEXITED(status_right))
    {
        return WEXITSTATUS(status_right);
    }

    return 1;
}
