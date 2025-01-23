#include "builtins_cd.h"

#include <stdio.h>
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
    if (argc == 1 && getVariable("HOME").value == NULL)
        return 1;

    if (argc == 1)
    {
        if (chdir(getVariable("HOME").value) == -1)
            return 1;
        update_pwd();
        return 0;
    }

    if (strcmp(argv[1], "-") == 0)
    {
        if (strcmp(getVariable("OLDPWD").value, "") == 0)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return 1;
        }
        if (chdir(getVariable("OLDPWD").value) == -1)
        {
            fprintf(stderr, "cd: No such file or directory\n");
            return 1;
        }
        printf("%s\n", getVariable("OLDPWD").value);
        update_pwd();

    }
    else if (chdir(argv[1]) == -1)
    {
        fprintf(stderr, "cd: %s: No such file or directory\n", argv[1]);
        return 1;
    }
    update_pwd();
    return 0;
}
