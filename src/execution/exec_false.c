#include <stdio.h>

#include "builtins_bool.h"

int exec_false(int argc, __attribute__((unused)) char **argv)
{
    if (argc > 1)
    {
        fprintf(stderr, "false: too many arguments\n");
        return 1;
    }

    return 1;
}
