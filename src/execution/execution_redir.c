#include "execution_redir.h"

#include <stdio.h>

// redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' )
// WORD ;
int exec_redir(struct ast *ast)
{
    (void)ast;
    return 0;
}
