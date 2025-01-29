#include "execution_redir.h"

#include <ctype.h>
#include <stdio.h>
#include <utils/word.h>

#include "execution.h"

static int aux_exec_redir(struct ast *ast);

// Remove the redirection from the AST by creating a new list starting at index
// 2 Obligated to do this because of the way the AST is built (which is not
// optimal for redirections)
static void removeRedir(struct ast *ast)
{
    free(ast->expanded_redir[0]);
    free(ast->expanded_redir[1]);
    ast->expanded_redir = ast->expanded_redir + 2;
}

// Handle >
static int simple_redir(char *filename, int io_number, struct ast *ast)
{
    if (io_number == -1)
    {
        io_number = STDOUT_FILENO;
    }

    int saved_fd = dup(io_number);
    if (saved_fd == -1)
    {
        return 1;
    }

    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        close(saved_fd);
        return 1;
    }

    if (dup2(fd, io_number) == -1)
    {
        close(fd);
        close(saved_fd);
        return 1;
    }
    close(fd);

    removeRedir(ast);
    int status = aux_exec_redir(ast);

    if (dup2(saved_fd, io_number) == -1)
    {
        close(saved_fd);
        return 1;
    }
    close(saved_fd);

    return status;
}


// Handle <
static int input_redir(char *filename, int io_number, struct ast *ast)
{
    if (io_number == -1)
    {
        io_number = STDIN_FILENO;
    }

    // Sauvegarder l'état initial du descripteur de fichier
    int saved_fd = dup(io_number);
    if (saved_fd == -1)
    {
        return 1;
    }

    // Ouvrir le fichier en lecture seule
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        close(saved_fd);
        return 1;
    }

    // Rediriger io_number vers fd
    if (dup2(fd, io_number) == -1)
    {
        close(fd);
        close(saved_fd);
        return 1;
    }
    close(fd);

    // Exécuter la commande
    removeRedir(ast);
    int status = aux_exec_redir(ast);

    // Restaurer l'état initial de io_number
    if (dup2(saved_fd, io_number) == -1)
    {
        close(saved_fd);
        return 1;
    }
    close(saved_fd);

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

    // Remove the redirection from the AST
    removeRedir(ast);
    // Execute the command
    int status = aux_exec_redir(ast);

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
    int saved_fd = dup(io_number);
    // Redirect the file descriptor
    if (dup2(fd, io_number) == -1)
    {
        close(fd);
        return 1;
    }
    // Close the file descriptor
    close(fd);
    // Remove the redirection from the AST
    removeRedir(ast);
    // Execute the command
    int status = aux_exec_redir(ast);
    // Restore the file descriptor
    if (dup2(saved_fd, io_number) == -1)
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
    int saved_fd = dup(io_number);
    // Redirect the file descriptor
    if (dup2(fd, io_number) == -1)
    {
        close(fd);
        return 1;
    }
    // Close the file descriptor
    close(fd);
    // Remove the redirection from the AST
    removeRedir(ast);
    // Execute the command
    int status = aux_exec_redir(ast);
    // Restore the file descriptor
    if (dup2(saved_fd, io_number) == -1)
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

    // Remove the redirection from the AST
    removeRedir(ast);
    // Execute the command
    int status = aux_exec_redir(ast);

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

static int has_redir(struct ast *ast)
{
    for (char *redir = ast->expanded_redir[0]; *redir; redir++)
    {
        if (*redir == '>' || *redir == '<')
        {
            return 1;
        }
    }
    return 0;
}

static int aux_exec_redir(struct ast *ast)
{
    if (!ast->expanded_redir[0] || !has_redir(ast))
    {
        // Free the redirections
        for (size_t i = 0; ast->redir[i]; i++)
        {
            word_free(ast->redir[i]);
        }
        free(ast->redir);
        ast->redir = NULL;
        return execute_node(ast);
    }

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

// redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' )
// WORD ;
int exec_redir(struct ast *ast)
{
    char **redir = ast->expanded_redir;
    int status = aux_exec_redir(ast);
    free(redir);
    return status;
}
