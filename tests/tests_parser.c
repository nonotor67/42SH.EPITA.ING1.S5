#include <ast/ast.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <unistd.h>

Test(parser, test_parser_simple_command)
{
    char *argv[] = { "42sh", "-c", "echo Hello World" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct parser parser = { PARSER_OK };
    struct ast *ast = parse(&parser, lexer);
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, SIMPLE_COMMAND);
    cr_assert_not_null(ast->values);
    cr_assert_str_eq(ast->values[0], "echo");
    cr_assert_str_eq(ast->values[1], "Hello");
    cr_assert_str_eq(ast->values[2], "World");
    cr_assert_null(ast->values[3]);
    ast_free(ast);
    lexer_free(lexer);
    reader_free(reader);
}

Test(parser, test_parser_command_list)
{
    char *argv[] = { "42sh", "-c", "echo Hello ; echo World" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct parser parser = { PARSER_OK };
    struct ast *ast = parse(&parser, lexer);
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, COMMAND_LIST);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, COMMAND_LIST);
    cr_assert_eq(ast->right->left->type, SIMPLE_COMMAND);

    cr_assert_str_eq(ast->left->values[0], "echo");
    cr_assert_str_eq(ast->left->values[1], "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->right->left->values[0], "echo");
    cr_assert_str_eq(ast->right->left->values[1], "World");
    cr_assert_null(ast->right->left->values[2]);
    ast_free(ast);
    lexer_free(lexer);
    reader_free(reader);
}