#include <stdio.h>
#include <stdlib.h>

#include "builtins_bool.h"

int exec_true(int argc, char **argv)
{
    if (argc > 1)
    {
        fprintf(stderr, "true: too many arguments\n");
        exit(1);
    }

    return 0;
}
