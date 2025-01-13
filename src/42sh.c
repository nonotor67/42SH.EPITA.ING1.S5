#include <ast/ast.h>
#include <bits/getopt_core.h>
#include <execution/execution.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <parser/parser.h>

#define DEBUG(str)                                                             \
    if (verbose)                                                               \
    printf("%s\n", str)

int execute_loop(struct lexer *lexer, char **argv)
{
    // TODO: populate $n with argv
    (void)argv; // avoid unused
    int res = 0;
    while (lexer->current.type != TOKEN_EOF)
    {
        struct parser *parser = parser_new(lexer);

        // Parse the input.
        struct ast *ast = parse(parser);

        // Execute the AST.
        if (ast)
            res = execution(ast);
        if (parser->status == PARSER_BAD_IF)
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
    int real_argc = argc - optind + 1;
    char **real_argv = argv + optind;
    // Create a reader
    struct reader *reader;
    if (real_argc == 1) // no arg
        reader = reader_from_stdin();
    else if (cflag) // -c str [ARGUMENTS...]
        // skip the first argument (the command)
        reader = reader_from_string(*real_argv++);
    else // [ARGUMENTS...] (first one is the file)
        reader = reader_from_file(*real_argv);
    DEBUG("Created reader");

    struct lexer *lexer = lexer_new(reader);
    DEBUG("Created lexer");

    // Default
    int res = execute_loop(lexer, real_argv);
    DEBUG("Done executing loop");
    reader_free(reader);
    lexer_free(lexer);

    return res;
}
