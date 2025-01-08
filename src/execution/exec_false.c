#include <stdio.h>
#include <stdlib.h>

#include "builtins_bool.h"

int exec_false(int argc, char **argv)
{
    if (argc > 1)
    {
        fprintf(stderr, "false: too many arguments\n");
        exit(1);
    }

    return 1;
}
