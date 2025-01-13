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
    // for ... in ... do ... done
    FOR_LOOP,
    // while ... do ... done
    WHILE_LOOP,
    // until ... do ... done
    UNTIL_LOOP,
    // ! ...
    NEGATION,
    // pipeline
    PIPELINE
};

struct ast
{
    enum ast_type type;
    struct ast *left;
    struct ast *middle;
    struct ast *right;
    int size;
    // values[0] is the name of the command for SIMPLE_COMMAND
    char **values;
    // Is used in simple commands, conditions, and loops
    char **redir;
};

// Create a new AST node (allocated)
struct ast *ast_new(enum ast_type type);

// Free the AST node
void ast_free(struct ast *ast);

// Save AST in DOT format
void ast_print(struct ast *ast);

/*
Example of AST:
echo hello
    type = SIMPLE_COMMAND
    left = NULL
    middle = NULL
    right = NULL
    size = 2
    values = ["echo", "hello", NULL]

echo hello ; echo world
    type = COMMAND_LIST
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "hello", NULL]
    middle = NULL
    right = SIMPLE_COMMAND
        size = 2
        values = ["echo", "world", NULL]
    size = 0
    values = NULL

echo hello ; echo world ; echo !
    type = COMMAND_LIST
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "hello", NULL]
    middle = NULL
    right = COMMAND_LIST
        left = SIMPLE_COMMAND
            size = 2
            values = ["echo", "world", NULL]
        middle = NULL
        right = COMMAND_LIST
            left = SIMPLE_COMMAND
                size = 2
                values = ["echo", "!", NULL]
            middle = NULL
            right = NULL
        size = 0
        values = NULL
    size = 0
    values = NULL

if echo hello ; then echo world ; else echo bye ; fi
    type = CONDITIONS
    left = COMMAND_LIST
        left = SIMPLE_COMMAND
            size = 2
            values = ["echo", "hello", NULL]
        middle = NULL
        right = NULL
        size = 0
        values = NULL
    middle = COMMAND_LIST
        left = SIMPLE_COMMAND
            size = 2
            values = ["echo", "world", NULL]
        middle = NULL
        right = NULL
        size = 0
        values = NULL
    right = COMMAND_LIST
        left = SIMPLE_COMMAND
            size = 2
            values = ["echo", "bye", NULL]
        middle = NULL
        right = NULL
        size = 0
        values = NULL
    size = 0
    values = NULL

if echo hello ; then echo world ; elif echo Test ; then echo encore ; else echo
! fi
    type = CONDITIONS
    left = COMMAND_LIST
        left = SIMPLE_COMMAND
            size = 2
            values = ["echo", "hello", NULL]
            middle = NULL right = NULL
    size = 0
    values = NULL
    middle =  COMMAND_LIST
        left = SIMPLE_COMMAND
            size = 2
            values = ["echo", "world", NULL]
        middle = NULL
        right = NULL
        size = 0
        values = NULL
    right = CONDITIONS
        left = COMMAND_LIST
            left = SIMPLE_COMMAND
                size = 2
                values = ["echo", "Test", NULL]
            middle = NULL
            right = NULL
            size = 0
            values = NULL
        middle = COMMAND_LIST
            left = SIMPLE_COMMAND
                size = 2
                values = ["echo", "encore", NULL]
            middle = NULL
            right = NULL
            size = 0
            values = NULL
        right = SIMPLE_COMMAND
            size = 2
            values = ["echo", "!", NULL]
        size = 0
        values = NULL
    size = 0
    values = NULL

*/

#endif /* !AST_H */
