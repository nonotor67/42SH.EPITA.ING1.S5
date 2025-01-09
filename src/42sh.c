#include <ast/ast.h>
#include <execution/execution.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <parser/parser.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    // Create a reader, lexer, and parser.
    struct reader *reader = reader_new(argc, argv);
    struct lexer *lexer = lexer_new(reader);
    struct parser *parser = parser_new(lexer);

    // Parse the input.
    struct ast *ast = parse(parser);

    // Execute the AST.
    int res = execution(ast);

    reader_free(reader);
    lexer_free(lexer);
    parser_free(parser);
    ast_free(ast);

    return res;
}
