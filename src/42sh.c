#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io/io.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "ast/ast.h"
#include "execution/execution.h"

int main(int argc, char** argv)
{
    //Create a reader, lexer, and parser.
    struct reader *reader = reader_new(argc, argv);
    struct lexer *lexer = lexer_new(reader);
    struct parser *parser = parser_new(lexer);

    //Parse the input.
    struct ast *ast = parse(parser);

    //Execute the AST.
    return execution(ast);
}
