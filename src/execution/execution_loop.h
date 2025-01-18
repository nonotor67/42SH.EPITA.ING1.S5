#ifndef EXECUTION_LOOP_H
#define EXECUTION_LOOP_H

#include "ast/ast.h"

int execution_while(struct ast *ast);
int execution_until(struct ast *ast);
int execution_for(struct ast *ast);

#endif /* !EXECUTION_LOOP_H */
