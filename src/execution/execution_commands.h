#ifndef EXECUTION_COMMANDS_H
#define EXECUTION_COMMANDS_H

#include <string.h>

#include "ast/ast.h"
#include "builtins_bool.h"
#include "builtins_echo.h"
#include "execution_redir.h"

/*
@brief: Execute all commands
@params:
    size: size of the values array
    values: array of command arguments ending with NULL
@return:
    exit status of the command
 */
int dispatch_command(int size, char **values);

/*
@params:
    ast: AST node of type SIMPLE_COMMAND
@return:
    exit status of the command
 */
int execute_command(struct ast *ast);

/*
@params:
    ast: AST node of type COMMAND_LIST
@return:
    exit status of the last command in the list
 */
int execute_command_list(struct ast *ast);

#endif /* !EXECUTION_COMMANDS_H */
