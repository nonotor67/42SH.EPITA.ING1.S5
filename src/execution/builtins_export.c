#define _POSIX_C_SOURCE 200112L

#include "builtins_export.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/hashMapVar.h>

#include "utils/xmalloc.h"

int exec_export(__attribute__((unused)) int argc, char **argv)
{
    char *tmp = strchr(argv[1], '=');
    if (tmp == NULL)
        return 1;

    char *name = xmalloc((tmp - argv[1]) + 1);
    strncpy(name, argv[1], tmp - argv[1]);
    name[tmp - argv[1]] = '\0';

    setenv(name, tmp + 1, 1);
    free(name);

    return 0;
}

int exec_unset(__attribute__((unused)) int argc, char **argv)
{
    if (getenv(argv[1]) == NULL)
        return 1;
    unsetenv(argv[1]);
    removeVariable(argv[1]);
    return 0;
}
