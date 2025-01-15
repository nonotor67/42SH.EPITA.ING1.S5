#ifndef EXECUTION_NEGATION_H
#define EXECUTION_NEGATION_H

#include <ast/ast.h>

#include "execution.h"

/*
@brief: Executes a negation node
@param node: The node to execute
@return: The exit code negated
*/
int execute_negation(struct ast *node);

#endif /* ! EXECUTION_NEGATION_H */
