#ifndef EXECUTION_FUNCTION_H
#define EXECUTION_FUNCTION_H

#include <ast/ast.h>
#include <utils/hashMapFunc.h>
#include <utils/hashMapVar.h>

#include "execution.h"

int executution_function(struct ast *ast);

int run_function(int argc, char *argv[]);

#endif /* ! EXECUTION_FUNCTION_H */
