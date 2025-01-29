#include <stdio.h>
#include <stdlib.h>
#include <utils/hashMapVar.h>

#include "execution.h"

void exec_exit(int argc, char **argv)
{
    if (argc > 2)
    {
        fprintf(stderr, "exit: too many arguments\n");
    }
    else if (argc > 1)
    {
        int exit_code = atoi(argv[1]);
        if (exit_code < 0 || exit_code > 255)
        {
            exit_code = 255;
        }
        exit(exit_code);
    }
    else
    {
        exit(getVariable("?").value == NULL ? 0 : atoi(getVariable("?").value));
    }
}
