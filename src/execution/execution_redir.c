#include "execution_redir.h"

#include <ctype.h>
#include <stdio.h>

#include "execution.h"

// Handle >
static int simple_redir(char *filename, int io_number, struct ast *ast)
{
    if (io_number == -1)
    {
        io_number = STDOUT_FILENO;
    }
    // Remove the file if it exists
    if (access(filename, F_OK) != -1)
    {
        remove(filename);
    }
    // Create and open the file
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    // Redirect the file descriptor
    if (dup2(fd, io_number) == -1)
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    // Close the file descriptor
    close(fd);
    // Execute the command
    int status = dispatch_command(ast);
    // Restore the file descriptor
    if (dup2(STDOUT_FILENO, io_number) == -1)
    {
        perror("dup2");
        return -1;
    }

    return status;
}

// Handle <
static int input_redir(char *filename, int io_number, struct ast *ast)
{
    if (io_number == -1)
    {
        io_number = STDIN_FILENO;
    }
    // Open the file
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    // Redirect the file descriptor
    if (dup2(fd, io_number) == -1)
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    // Close the file descriptor
    close(fd);
    // Execute the command
    int status = dispatch_command(ast);
    // Restore the file descriptor
    if (dup2(STDIN_FILENO, io_number) == -1)
    {
        perror("dup2");
        return -1;
    }

    return status;
}

// Handle >&
static int fd_redir(int fd, int io_number, struct ast *ast)
{
    if (io_number == -1)
    {
        io_number = STDOUT_FILENO;
    }
    // Redirect the file descriptor
    if (dup2(fd, io_number) == -1)
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    // Close the file descriptor
    close(fd);
    // Execute the command
    int status = dispatch_command(ast);
    // Restore the file descriptor
    if (dup2(STDOUT_FILENO, io_number) == -1)
    {
        perror("dup2");
        return -1;
    }

    return status;
}

// redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' )
// WORD ;
int exec_redir(struct ast *ast)
{
    int io_number = -1;
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

    int status = 1;
    if (strcmp(ast->expanded_redir[i], ">") == 0
        || strcmp(ast->expanded_redir[i], ">|") == 0)
    {
        status = simple_redir(ast->expanded_redir[i + 1], io_number, ast);
    }
    else if (strcmp(ast->expanded_redir[i], ">&") == 0)
    {
        status = fd_redir(atoi(ast->expanded_redir[i + 1]), io_number, ast);
    }
    else if (strcmp(ast->expanded_redir[i], "<>") == 0)
    {
    }
    else if (strcmp(ast->expanded_redir[i], "<") == 0)
    {
        input_redir(ast->expanded_redir[i + 1], io_number, ast);
    }
    else if (strcmp(ast->expanded_redir[i], ">>") == 0)
    {
    }
    else if (strcmp(ast->expanded_redir[i], "<&") == 0)
    {
    }
    return status;
}
