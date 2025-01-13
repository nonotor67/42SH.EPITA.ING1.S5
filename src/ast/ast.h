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
    WORD_COMPONENTS,
    // while ... do ... done
    WHILE_LOOP,
    // until ... do ... done
    UNTIL_LOOP,
    // pipeline
    PIPELINE,
    // Operators
    AND,
    NEGATION,
    OR
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
Step 1 :

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

Step 2 :

echo test > a.out
    type = COMMAND_LIST
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "test", NULL]
        redir = [">", "a.out", NULL]
    middle = NULL
    right = NULL
    size = 0
    values = NULL

echo test > a.out 2> b.out
    type = COMMAND_LIST
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "test", NULL]
        redir = [">", "a.out", "2>", "b.out", NULL]
    middle = NULL
    right = NULL
    size = 0
    values = NULL

---> Pipeline works the same as List
--> You just need to connect stdout of the left command to
--> the stdin of the left child of the right command (if any)

echo test | cat
    type = PIPELINE
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "test", NULL]
    middle = NULL
    right = PIPELINE
        left = SIMPLE_COMMAND
            size = 1
            values = ["cat", NULL]
        middle = NULL
        right = NULL
    size = 0
    values = NULL

echo test | if echo hello ; then echo world ; else echo bye ; fi
    type = PIPELINE
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "test", NULL]
    middle = NULL
    right = PIPELINE
        left = CONDITION
            left = COMMAND_LIST
                left = SIMPLE_COMMAND
                    size = 2
                    values = ["echo", "hello", NULL]
            middle = COMMAND_LIST
                left = SIMPLE_COMMAND
                    size = 2
                    values = ["echo", "world", NULL]
            right = COMMAND_LIST
                left = SIMPLE_COMMAND
                    size = 2
                    values = ["echo", "bye", NULL]
            size = 0
            values = NULL
        middle = NULL
        right = NULL
        size = 0
        values = NULL

! echo test **Simplified**
    type = NOT
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "test", NULL]
    middle = NULL
    right = NULL
    size = 0
    values = NULL

echo test && echo hello **Simplified**
    type = AND
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "test", NULL]
    middle = NULL
    right = SIMPLE_COMMAND
        size = 2
        values = ["echo", "hello", NULL]
    size = 0
    values = NULL

echo test || echo hello **Simplified**
    type = OR
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "test", NULL]
    middle = NULL
    right = SIMPLE_COMMAND
        size = 2
        values = ["echo", "hello", NULL]
    size = 0
    values = NULL

---> Loops
--> While
-> left is condition
-> right is the list of commands to execute

while echo test ; do echo hello ; done **Simplified**
    type = WHILE_LOOP
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "test", NULL]
    middle = NULL
    right = COMMAND_LIST
        left = SIMPLE_COMMAND
            size = 2
            values = ["echo", "hello", NULL]
        middle = NULL
        right = NULL
    size = 0
    values = NULL

--> Until
-> left is condition
-> right is the list of commands to execute

until echo test ; do echo hello ; done **Simplified**
    type = UNTIL_LOOP
    left = SIMPLE_COMMAND
        size = 2
        values = ["echo", "test", NULL]
    middle = NULL
    right = COMMAND_LIST
        left = SIMPLE_COMMAND
            size = 2
            values = ["echo", "hello", NULL]
        middle = NULL
        right = NULL
    size = 0
    values = NULL

--> For
-> left is the variable
-> middle is the list of values
-> right is the list of commands to execute

for i in 1 2 3 ; do echo $i ; done **Simplified**
    type = FOR_LOOP
    left = WORD_COMPONENTS
        size = 1
        values = ["i", NULL]
    middle = WORD_COMPONENTS
        size = 3
        values = ["1", "2", "3", NULL]
    right = COMMAND_LIST
        left = SIMPLE_COMMAND
            size = 2
            values = ["echo", "$i", NULL]
        middle = NULL
        right = NULL
    size = 0
    values = NULL
*/

#endif /* !AST_H */
