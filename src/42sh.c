#include <ast/ast.h>
#include <bits/getopt_core.h>
#include <execution/execution.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>

#define DEBUG(str)                                                             \
    if (verbose)                                                               \
    printf("%s\n", str)

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

        char *buf = xmalloc(256);
        insertVariable("?", my_itoa(res, buf));
        free(buf);

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
