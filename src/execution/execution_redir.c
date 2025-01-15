#include "execution_redir.h"

#include <ctype.h>
#include <stdio.h>

#include "execution.h"

static int simple_redir(char *filename, int io_number, struct ast *ast)
{
    // Remove the file if it exists
    if (access(filename, F_OK) != -1)
    {
        remove(filename);
    }
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        exit(1);
    }
    int dupstdout = dup(STDOUT_FILENO);
    if (dupstdout == -1)
    {
        close(fd);
        exit(1);
    }
    int res = dup2(fd, io_number);
    if (res == -1)
    {
        close(fd);
        close(dupstdout);
        exit(1);
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        exit(1);
    }
    if (pid == 0)
    {
        exit(dispatch_command(ast));
    }

    int status;
    waitpid(pid, &status, 0);
    res = dup2(dupstdout, io_number);
    if (res == -1)
    {
        close(dupstdout);
        exit(1);
    }

    close(dupstdout);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
    {
        exit(1);
    }
    return status;
}

// redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' )
// WORD ;
int exec_redir(struct ast *ast)
{
    int io_number = 1;
    int i = 0;
    // Parse io number
    if (isdigit(ast->expanded_redir[0][0]))
    {
        io_number = ast->expanded_redir[0][0] - '0';
        // Remove the io number from the list
        for (i = 0; i < ast->redir_size - 2; i++)
        {
            ast->expanded_redir[i] = ast->expanded_redir[i + 2];
        }
    }
    // Handle >
    int status = 1;
    if (strcmp(ast->expanded_redir[i], ">") == 0)
    {
        status = simple_redir(ast->expanded_redir[i + 1], io_number, ast);
    }

    return status;
}
