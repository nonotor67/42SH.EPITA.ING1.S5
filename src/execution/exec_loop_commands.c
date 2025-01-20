#include "builtins_loop.h"

#include <stdlib.h>
#include <stdio.h>

#include "execution_loop.h"

void exec_continue(int argc, char **argv)
{
    (void)argc;
    int occurence = 1;
    if (argc == 2)
    {
        occurence = atoi(argv[1]);
    }
    if (loop_status)
    {
        loop_status->status = CONTINUE_LOOP;
        loop_status->occurence = occurence;
    }
    else
    {
        fprintf(stderr, "continue: only meaningful in a loop\n");
    }
}

void exec_break(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    if (loop_status)
    {
        loop_status->status = BREAK_LOOP;
    }
    else
    {
        fprintf(stderr, "break: only meaningful in a loop\n");
    }
}
