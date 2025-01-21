#include <stdio.h>
#include <stdlib.h>

#include "builtins_loop.h"
#include "execution_loop.h"

void exec_continue(int argc, char **argv)
{
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

void exec_break(__attribute__((unused)) int argc,
                __attribute__((unused)) char **argv)
{
    if (loop_status)
    {
        loop_status->status = BREAK_LOOP;
    }
    else
    {
        fprintf(stderr, "break: only meaningful in a loop\n");
    }
}
