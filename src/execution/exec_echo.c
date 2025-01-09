#include <getopt.h>
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

        if (argv[idx][1] == 'n')
        {
            nflag = true;
        }
        else if (argv[idx][1] == 'e')
        {
            eflag = true;
        }
        else if (argv[idx][1] == 'E')
        {
            eflag = false;
        }
        else
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
