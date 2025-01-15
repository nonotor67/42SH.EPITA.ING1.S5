#ifndef EXECUTION_H
#define EXECUTION_H

#include <ast/ast.h>

#include "builtins_bool.h"
#include "builtins_echo.h"
#include "builtins_exit.h"
#include "execution_commands.h"
#include "execution_conditions.h"
#include "execution_negation.h"
#include "execution_or_and.h"
#include "execution_pipeline.h"
#include "execution_while.h"

/*
@description:
    Execute a node.
@param:
    struct ast *node: The condition node.
@return:
    int: The return status of the node.
 */
int execute_node(struct ast *node);

int execution(struct ast *ast);

#endif /* !EXECUTION_H */
