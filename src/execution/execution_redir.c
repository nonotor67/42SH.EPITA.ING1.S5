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
        return 1;
    }
    // Redirect the file descriptor
    if (dup2(fd, io_number) == -1)
    {
        close(fd);
        return 1;
    }
    // Close the file descriptor
    close(fd);
    // Execute the command
    int status = dispatch_command(ast);
    // Restore the file descriptor
    if (dup2(STDOUT_FILENO, io_number) == -1)
    {
        return 1;
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
        return 1;
    }
    // Redirect the file descriptor
    if (dup2(fd, io_number) == -1)
    {
        close(fd);
        return 1;
    }
    // Close the file descriptor
    close(fd);
    // Execute the command
    int status = dispatch_command(ast);
    // Restore the file descriptor
    if (dup2(STDIN_FILENO, io_number) == -1)
    {
        return 1;
    }

    return status;
}

// Handle >>
static int append_redir(char *filename, int io_number, struct ast *ast)
{
    if (io_number == -1)
    {
        io_number = STDOUT_FILENO;
    }

    // Ouvrir le fichier
    int fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd == -1)
    {
        return 1;
    }

    // Sauvegarder l'ancien descripteur
    int saved_stdout = dup(io_number);
    if (saved_stdout == -1)
    {
        close(fd);
        return 1;
    }

    // Rediriger la sortie standard
    if (dup2(fd, io_number) == -1)
    {
        close(fd);
        close(saved_stdout);
        return 1;
    }

    // Fermer le descripteur de fichier redirigé
    close(fd);

    // Exécuter la commande
    int status = dispatch_command(ast);

    // Restaurer la sortie standard
    if (dup2(saved_stdout, io_number) == -1)
    {
        close(saved_stdout);
        return 1;
    }

    // Fermer le descripteur sauvegardé
    close(saved_stdout);

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
        close(fd);
        return 1;
    }
    // Close the file descriptor
    close(fd);
    // Execute the command
    int status = dispatch_command(ast);
    // Restore the file descriptor
    if (dup2(STDOUT_FILENO, io_number) == -1)
    {
        return 1;
    }

    return status;
}

// Handle <&
static int fd_input_redir(int fd, int io_number, struct ast *ast)
{
    if (io_number == -1)
    {
        io_number = STDIN_FILENO;
    }
    // Redirect the file descriptor
    if (dup2(fd, io_number) == -1)
    {
        close(fd);
        return 1;
    }
    // Close the file descriptor
    close(fd);
    // Execute the command
    int status = dispatch_command(ast);
    // Restore the file descriptor
    if (dup2(STDIN_FILENO, io_number) == -1)
    {
        return 1;
    }

    return status;
}

// Handle <>, read and write
static int fd_write_read_redir(int fd, int io_number, struct ast *ast)
{
    if (io_number == -1)
    {
        io_number = STDIN_FILENO;
    }

    // Sauvegarder l'ancien descripteur
    int saved_fd = dup(io_number);
    if (saved_fd == -1)
    {
        close(fd);
        return 1;
    }

    // Rediriger le descripteur de fichier
    if (dup2(fd, io_number) == -1)
    {
        close(fd);
        close(saved_fd);
        return 1;
    }

    // Fermer le descripteur redirigé
    close(fd);

    // Exécuter la commande
    int status = dispatch_command(ast);

    // Restaurer l'ancien descripteur
    if (dup2(saved_fd, io_number) == -1)
    {
        close(saved_fd);
        return 1;
    }

    // Fermer le descripteur sauvegardé
    close(saved_fd);

    return status;
}

// redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' )
int exec_redir(struct ast *ast)
{
    int io_number = -1;

    // Parse IO number directly from the first string
    char *redir_op = ast->expanded_redir[0];
    char *start = redir_op;

    while (isdigit(*start)) // Skip digits
    {
        if (io_number == -1)
            io_number = 0;
        io_number = io_number * 10 + (*start - '0');
        start++;
    }

    // If digits were found, adjust redir_op to point after them
    if (start != redir_op)
    {
        redir_op = start;
    }

    // Handle redirection operators
    char *target = ast->expanded_redir[1];

    if (strcmp(redir_op, ">") == 0 || strcmp(redir_op, ">|") == 0)
    {
        return simple_redir(target, io_number, ast);
    }
    else if (strcmp(redir_op, ">&") == 0)
    {
        return fd_redir(atoi(target), io_number, ast);
    }
    else if (strcmp(redir_op, "<>") == 0)
    {
        return fd_write_read_redir(atoi(target), io_number, ast);
    }
    else if (strcmp(redir_op, "<") == 0)
    {
        return input_redir(target, io_number, ast);
    }
    else if (strcmp(redir_op, ">>") == 0)
    {
        return append_redir(target, io_number, ast);
    }
    else if (strcmp(redir_op, "<&") == 0)
    {
        return fd_input_redir(atoi(target), io_number, ast);
    }

    return 1;
}
