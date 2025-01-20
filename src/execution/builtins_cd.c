#include "builtins_cd.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils/utils.h"

static void update_pwd(void)
{
    char *pwd = getcwd(NULL, 0);
    insertVariable("OLDPWD", getVariable("PWD").value);
    insertVariable("PWD", pwd);
    free(pwd);
}

int exec_cd(int argc, char **argv)
{
    if (argc == 1 && getenv("HOME") == NULL)
        return 1;

    if (argc == 1)
    {
        if (chdir(getenv("HOME")) == -1)
            return 1;
        update_pwd();
        return 0;
    }

    if (chdir(argv[1]) == -1)
        return 1;

    return 0;
}
