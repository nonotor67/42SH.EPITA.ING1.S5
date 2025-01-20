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

    while (idx < argc)
    {
        if (argv[idx][0] != '-')
        {
            break;
        }

        if (argv[idx][1] == '\0')
        {
            break;
        }

        int i = 1;
        int flag = 0;
        while (argv[idx][i] != '\0')
        {
            if (argv[idx][i] == 'n')
            {
                nflag = true;
            }
            else if (argv[idx][i] == 'e')
            {
                eflag = true;
            }
            else if (argv[idx][i] == 'E')
            {
                eflag = false;
            }
            else
            {
                flag = 1;
            }

            if (flag)
            {
                break;
            }
            i++;
        }
        if (flag)
        {
            break;
        }

        idx++;
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
    if (eflag)
    {
        for (const char *p = arg; *p; p++)
        {
            if (*p == '\\')
            {
                while (*(p + 1) == '\\')
                {
                    p++;
                }
                print_escaped_char(&p);
            }
            else
            {
                putchar(*p);
            }
        }
    }
    else
    {
        printf("%s", arg);
    }
}

static void print_escaped_char(const char **p)
{
    (*p)++;
    if (**p == 'n')
    {
        printf("\n");
    }
    else if (**p == 't')
    {
        printf("\t");
    }
    else
    {
        putchar('\\');
        putchar(**p);
    }
}
