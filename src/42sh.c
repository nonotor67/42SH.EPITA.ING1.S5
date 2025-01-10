#include <ast/ast.h>
#include <execution/execution.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <parser/parser.h>

int main(int argc, char **argv)
{
    // Create a reader, lexer, and parser.
    struct reader *reader = reader_new(argc, argv);
    struct lexer *lexer = lexer_new(reader);
    int res = 1;
    while (lexer->current.type != TOKEN_EOF)
    {
        struct parser *parser = parser_new(lexer);

        // Parse the input.
        struct ast *ast = parse(parser);

        // Execute the AST.
        res = execution(ast);
        parser_free(parser);
        ast_free(ast);
    }
    

    reader_free(reader);
    lexer_free(lexer);

    return res;
}
