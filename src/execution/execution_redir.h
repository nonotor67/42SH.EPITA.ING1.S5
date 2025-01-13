#ifndef EXECUTION_REDIR_H
#define EXECUTION_REDIR_H

#include <ast/ast.h>
#include "execution_commands.h"

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

/*
@brief Execute a command with redirections
@param size: size of the values array
@param values: array of command arguments ending with NULL
@param redir: array of redirections ending with NULL
@return exit status of the command
 */
int exec_redir(int size, char **values, char **redir);

#endif /* ! EXECUTION_REDIR_H */
