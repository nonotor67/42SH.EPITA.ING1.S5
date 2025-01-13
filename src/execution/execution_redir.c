#include "execution_redir.h"

// redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' ) WORD ;
int exec_redir(int size, char **values, char **redir)
{
    int fd = -1;
    int status = 0;
    for (int i = 0; redir[i]; i += 2)
    {
        if (redir[i][0] == '>')
        {
            if (redir[i][1] == '>')
                fd = open(redir[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0644);
            else
                fd = open(redir[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        }
        else if (redir[i][0] == '<')
            fd = open(redir[i + 1], O_RDONLY);
        else if (redir[i][0] == '2' && redir[i][1] == '>')
            fd = open(redir[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        else if (redir[i][0] == '<' && redir[i][1] == '&')
            fd = open(redir[i + 1], O_RDONLY);
        else if (redir[i][0] == '>' && redir[i][1] == '&')
            fd = open(redir[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        else if (redir[i][0] == '<' && redir[i][1] == '>')
            fd = open(redir[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        else if (redir[i][0] == '>' && redir[i][1] == '|')
            fd = open(redir[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        else if (redir[i][0] == '<' && redir[i][1] == '>')
            fd = open(redir[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd == -1)
        {
            return 1;
        }
        if (dup2(fd, redir[i][0] == '<' ? STDIN_FILENO : STDOUT_FILENO) == -1)
        {
            return 1;
        }
        close(fd);
    }
    return dispatch_command(size, values);
}
