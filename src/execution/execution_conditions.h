#ifndef EXECUTION_CONDITIONS_H
#define EXECUTION_CONDITIONS_H

#include "ast/ast.h"
#include "execution.h"

/*
@description:
    Execute the condition node.
@param:
    struct ast *ast: The condition node.
@return:
    int: The return status of the condition node.
 */

int execute_condition(struct ast *ast);

#endif /* !EXECUTION_CONDITIONS_H */
