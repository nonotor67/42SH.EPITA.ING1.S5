#ifndef AST_H
#define AST_H

#include <unistd.h>

enum ast_type
{
    // echo ...
    SIMPLE_COMMAND,
    // echo ... ; echo ...
    COMMAND_LIST,
    // if ... then ... else ... fi
    CONDITIONS,
};

struct ast
{
    enum ast_type type;
    struct ast *left;
    struct ast *right;
    char *value;
    char **values;
};

// Create a new AST node (allocated)
struct ast *ast_new(enum ast_type type);

// Free the AST node
void ast_free(struct ast *ast);

#endif // AST_H