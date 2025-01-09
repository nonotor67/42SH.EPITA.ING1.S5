#include <ast/ast.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <unistd.h>

Test(parser, test_parser)
{
    char *argv[] = { "42sh", "-c", "echo Hello World" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    enum parser_status status = PARSER_OK;
    struct ast *ast = parse(&status, lexer);
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, SIMPLE_COMMAND);
    cr_assert_not_null(ast->values);
    cr_assert_str_eq(ast->values[0], "echo");
    cr_assert_str_eq(ast->values[1], "hello");
    cr_assert_str_eq(ast->values[2], "world");
    cr_assert_null(ast->values[3]);
    ast_free(ast);
    lexer_free(lexer);
}
