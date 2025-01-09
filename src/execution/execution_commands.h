#ifndef EXECUTION_COMMANDS_H
#define EXECUTION_COMMANDS_H

#include <ast/ast.h>
#include <string.h>

#include "builtins_bool.h"
#include "builtins_echo.h"

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

#endif /* ! EXECUTION_COMMANDS_H */
