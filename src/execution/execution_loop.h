
#ifndef EXECUTION_WHILE_H
#define EXECUTION_WHILE_H

#include "ast/ast.h"

int execution_while(struct ast *ast);
int execution_until(struct ast *ast);
int execution_for(struct ast *ast);

#endif /* !EXECUTION_WHILE_H */
