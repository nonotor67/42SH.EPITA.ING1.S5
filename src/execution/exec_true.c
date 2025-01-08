#include <stdio.h>
#include <stdlib.h>

#include "builtins_bool.h"

int exec_true(int argc, __attribute__((unused)) char **argv)
{
    if (argc > 1)
    {
        fprintf(stderr, "true: too many arguments\n");
        return 0;
    }

    return 0;
}
