#ifndef EXECUTION_SUBSHELL_H
#define EXECUTION_SUBSHELL_H

#include <ast/ast.h>
#include <utils/hashMapFunc.h>
#include <utils/hashMapVar.h>

#include "execution.h"

int execute_subshell(struct ast *ast);

#endif // EXECUTION_SUBSHELL_H
