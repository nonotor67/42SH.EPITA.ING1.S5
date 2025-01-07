#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "builtins_echo.h"

static void print_escaped_char(const char **p);
static void print_argument(const char *arg, bool eflag);

bool exec_echo(int argc, char **argv)
{
    int opt;
    bool nflag = false;
    bool eflag = false;

    while ((opt = getopt(argc, argv, "neE")) != -1)
    {
        switch (opt)
        {
        case 'n':
            nflag = true;
            break;
        case 'e':
            eflag = true;
            break;
        case 'E':
            eflag = false;
            break;
        default:
            return false;
        }
    }

    for (int i = optind; i < argc; i++)
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

    return true;
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
