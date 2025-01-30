#include <stdbool.h>
#include <stdio.h>

#include "execution.h"

static void print_escaped_char(const char **p);
static void print_argument(const char *arg, bool eflag);

bool exec_echo(int argc, char **argv)
{
    int idx = 1;
    bool nflag = false;
    bool eflag = false;

    // Parse options
    for (int i = 1; i < argc; i++)
    {
        bool flag = false;
        if (argv[i][0] == '-')
        {
            for (int j = 1; argv[i][j] != '\0'; j++)
            {
                if (argv[i][j] == 'n')
                {
                    nflag = true;
                }
                else if (argv[i][j] == 'e')
                {
                    eflag = true;
                }
                else if (argv[i][j] == 'E')
                {
                    eflag = false;
                }
                else
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
                break;
            idx++;
        }
        else
        {
            break;
        }
    }

    for (int i = idx; i < argc; i++)
    {
        print_argument(argv[i], eflag);
        if (i < argc - 1)
        {
            putchar(' ');
        }
    }

    if (!nflag)
    {
        putchar('\n');
    }

    fflush(stdout);

    return 0;
}

static void print_argument(const char *arg, bool eflag)
{
    const char *p = arg;
    while (*p)
    {
        if (*p == '\\')
        {
            if (eflag)
            {
                print_escaped_char(&p);
            }
            else
            {
                putchar(*p);
                p++;
            }
        }
        else
        {
            putchar(*p);
            p++;
        }
    }
}

static void print_escaped_char(const char **p)
{
    (*p)++;
    if (!*p || !**p || **p == '\0')
    {
        putchar('\\');
        return;
    }

    if (**p == 'n')
    {
        printf("\n");
    }
    else if (**p == 't')
    {
        printf("\t");
    }
    else if (**p == '\\')
    {
        printf("\\");
    }
    else
    {
        putchar(**p);
    }
    (*p)++;
}
