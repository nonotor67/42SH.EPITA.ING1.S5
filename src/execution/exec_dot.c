#include <execution/execution.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <stdio.h>
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
    char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return 1;
    }
    // Read the whole file
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = xmalloc(size + 1);
    size_t read = fread(buffer, 1, size, file);
    if (read != size)
    {
        fprintf(stderr, "Failed to read file: %s\n", filename);
        free(buffer);
        fclose(file);
        return 1;
    }
    buffer[size] = '\0';
    fclose(file);

    int res = exec_file(buffer);
    free(buffer);
    return res;
}
