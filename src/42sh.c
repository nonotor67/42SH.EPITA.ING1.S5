#include <ast/ast.h>
#include <bits/getopt_core.h>
#include <execution/execution.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <stdlib.h>
#include <string.h>
#include <utils/utils.h>

#include "utils/hashMap.h"

#define DEBUG(str)                                                             \
    if (verbose)                                                               \
    printf("%s\n", str)

static char *my_itoa(int value, char *s)
{
    if (value == 0)
    {
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    size_t i = 0;
    if (value < 0)
    {
        s[i] = '-';
        i++;
        value = -value;
    }
    long value_rev = 0;
    int nb_zero = 0;
    while (value != 0)
    {
        if (value % 10 == 0 && value_rev == 0)
            nb_zero++;
        value_rev *= 10;
        value_rev += value % 10;
        value /= 10;
    }
    for (; value_rev != 0; i++)
    {
        s[i] = value_rev % 10 + '0';
        value_rev /= 10;
    }
    for (; nb_zero > 0; i++)
    {
        s[i] = '0';
        nb_zero--;
    }
    s[i] = '\0';
    return s;
}

/**
 * Initialize the environment variables
 * @param argc The number of arguments
 * @param argv The arguments
 */
static void env_init(int argc, char **argv)
{
    char *argument_list = xmalloc(1024);
    unsigned int argument_list_size = 1024;
    unsigned int actual_list_size = 0;

    for (int i = 1; i < argc; i++)
    {
        if (actual_list_size + strlen(argv[i]) + 1 >= argument_list_size)
        {
            argument_list_size *= 2;
            argument_list = xrealloc(argument_list, argument_list_size);
        }
        strcpy(argument_list + actual_list_size, argv[i]);
        actual_list_size += strlen(argv[i]);
        argument_list[actual_list_size] = ' ';
        actual_list_size++;
        char *int_tmp = xmalloc(256);
        insertVariable(my_itoa(i, int_tmp), argv[i]);
        free(int_tmp);
    }

    insertVariable("0", argv[0]);

    if (actual_list_size > 0)
    {
        argument_list[actual_list_size - 1] = '\0';
        insertVariable("@", argument_list);
        insertVariable("*", argument_list);
    }
    free(argument_list);

    // InsertVariables allocate a new string, so we can free or use the old one
    char *int_tmp = xmalloc(256);
    insertVariable("#", my_itoa(argc, int_tmp));
    insertVariable("$", my_itoa(getpid(), int_tmp));
    insertVariable("UID", my_itoa(getuid(), int_tmp));
    free(int_tmp);

    insertVariable("?", "0");

    insertVariable("OLDPWD", getenv("PWD"));
    insertVariable("PWD", getenv("PWD"));
    insertVariable("IFS", " \t\n");
}

static int execute_loop(struct lexer *lexer, int real_argc, char **real_argv)
{
    env_init(real_argc, real_argv);
    int res = 0;
    while (lexer->current.type != TOKEN_EOF)
    {
        struct parser *parser = parser_new(lexer);

        // Parse the input.
        struct ast *ast = parse(parser);

        // Execute the AST.
        if (ast)
            res = execution(ast);
        if (parser->status != PARSER_OK && parser->status != PARSER_EOF)
            res = 2;
        parser_free(parser);
        ast_free(ast);
    }
    return res;
}

int main(int argc, char **argv)
{
    int verbose = 0;
    int cflag = 0;
    int c;
    while ((c = getopt(argc, argv, "cv")) != -1)
    {
        switch (c)
        {
        case 'c':
            cflag = 1;
            break;
        case 'v':
            verbose = 1;
            break;
        default:
            abort();
        }
    }
    int real_argc = argc - optind;
    char **real_argv = argv + optind;
    // Create a reader
    struct reader *reader;
    if (argc == 1) // no arg
    {
        reader = reader_from_stdin();
        real_argv[0] = argv[0];
    }
    else if (cflag) // -c str [ARGUMENTS...]
    {
        // skip the first argument (the command)
        reader = reader_from_string(*real_argv);
        real_argv[0] = argv[0];
    }
    else // [ARGUMENTS...] (first one is the file)
        reader = reader_from_file(*real_argv);
    DEBUG("Created reader");

    struct lexer *lexer = lexer_new(reader);
    DEBUG("Created lexer");

    // Default
    int res = execute_loop(lexer, real_argc, real_argv);

    DEBUG("Done executing loop");
    reader_free(reader);
    lexer_free(lexer);
    free_hash_map();

    return res;
}
