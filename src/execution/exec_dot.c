#define _POSIX_C_SOURCE 200809L

#include <execution/execution.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <stdio.h>
#include <string.h>
#include <utils/utils.h>


#include "builtins_dot.h"

/*
@brief: Execute the content of a file in our env
@param: char *content: the content of the file
@return: the return value of the execution
 */
static int exec_file(char *content)
{
    struct reader *reader = reader_from_string(content);
    struct lexer *lexer = lexer_new(reader);
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
    reader_free(reader);
    lexer_free(lexer);
    return res;
}

int exec_dot(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: . filename\n");
        return 2;
    }
    // Check if the file contains ./ or ../
    if (strstr(argv[1], "./") == NULL && strstr(argv[1], "../") == NULL)
    {
        fprintf(stderr, "Error: %s: Matheo said that is not allowed!\n", argv[1]);
        return 1;
    }
    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return 1;
    }

    char *buffer = NULL;
    size_t size = 0;
    FILE *stream = open_memstream(&buffer, &size);
    if (!stream)
    {
        fprintf(stderr, "Failed to open memstream\n");
        fclose(file);
        return 1;
    }

    char line[1024];
    while ((fgets(line, sizeof(line), file)) != NULL)
    {
        fputs(line, stream);
    }

    fclose(file);
    fflush(stream);
    fclose(stream);

    if (!buffer)
    {
        fprintf(stderr, "Failed to read file: %s\n", filename);
        return 1;
    }

    int res = exec_file(buffer);
    free(buffer);
    return res;
}

