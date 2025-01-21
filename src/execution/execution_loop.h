#ifndef EXECUTION_LOOP_H
#define EXECUTION_LOOP_H

#include "ast/ast.h"

#define CONTINUE_LOOP -1
#define BREAK_LOOP -2

// If the code is not in a loop, loop_status = NULL
// Else the struct is defined
extern struct loop_status *loop_status;

struct loop_status
{
    int status;
    int occurence;
};

int execution_while(struct ast *ast);
int execution_until(struct ast *ast);
int execution_for(struct ast *ast);

#endif /* !EXECUTION_LOOP_H */
