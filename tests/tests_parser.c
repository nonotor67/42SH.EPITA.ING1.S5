#include <ast/ast.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <unistd.h>

Test(parser, test_parser_simple_command)
{
    char *argv[] = { "42sh", "-c", "echo Hello World" };
    struct reader *reader = reader_new(3, argv);
    struct lexer *lexer = lexer_new(reader);
    struct parser *parser = parser_new(lexer);

    struct ast *ast = parse(parser);
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
    parser_free(parser);
}

Test(parser, test_parser_command_list)
{
    char *argv[] = { "42sh", "-c", "echo Hello ; echo World" };
    struct reader *reader = reader_new(3, argv);
    struct lexer *lexer = lexer_new(reader);
    struct parser *parser = parser_new(lexer);

    struct ast *ast = parse(parser);
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
    parser_free(parser);
}

Test(parser, test_parser_if)
{
    char *argv[] = { "42sh", "-c", "if echo Hello ; then echo World ; fi" };
    struct reader *reader = reader_new(3, argv);
    struct lexer *lexer = lexer_new(reader);
    struct parser *parser = parser_new(lexer);

    struct ast *ast = parse(parser);
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, CONDITIONS);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->middle);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->middle->type, SIMPLE_COMMAND);
    cr_assert_null(ast->right);
    cr_assert_str_eq(ast->left->values[0], "echo");
    cr_assert_str_eq(ast->left->values[1], "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->middle->values[0], "echo");
    cr_assert_str_eq(ast->middle->values[1], "World");
    cr_assert_null(ast->middle->values[2]);

    ast_free(ast);
    lexer_free(lexer);
    reader_free(reader);
    parser_free(parser);
}

Test(parser, test_parser_if_else)
{
    char *argv[] = { "42sh", "-c", "if echo Hello ; then echo World ; else echo Bye ; fi" };
    struct reader *reader = reader_new(3, argv);
    struct lexer *lexer = lexer_new(reader);
    struct parser *parser = parser_new(lexer);

    struct ast *ast = parse(parser);
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, CONDITIONS);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->middle);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->middle->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0], "echo");
    cr_assert_str_eq(ast->left->values[1], "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->middle->values[0], "echo");
    cr_assert_str_eq(ast->middle->values[1], "World");
    cr_assert_null(ast->middle->values[2]);
    cr_assert_str_eq(ast->right->values[0], "echo");
    cr_assert_str_eq(ast->right->values[1], "Bye");
    cr_assert_null(ast->right->values[2]);
    ast_free(ast);
    lexer_free(lexer);
    reader_free(reader);
    parser_free(parser);
}

Test(parser, test_parser_if_elif)
{
    char *argv[] = { "42sh", "-c", "if echo Hello ; then echo World ; elif echo Bye ; then echo Test ; fi" };
    struct reader *reader = reader_new(3, argv);
    struct lexer *lexer = lexer_new(reader);
    struct parser *parser = parser_new(lexer);

    struct ast *ast = parse(parser);
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, CONDITIONS);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->middle);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->middle->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, CONDITIONS);
    cr_assert_not_null(ast->right->left);
    cr_assert_not_null(ast->right->middle);
    cr_assert_eq(ast->right->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->middle->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0], "echo");
    cr_assert_str_eq(ast->left->values[1], "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->middle->values[0], "echo");
    cr_assert_str_eq(ast->middle->values[1], "World");
    cr_assert_null(ast->middle->values[2]);
    cr_assert_str_eq(ast->right->left->values[0], "echo");
    cr_assert_str_eq(ast->right->left->values[1], "Bye");
    cr_assert_null(ast->right->left->values[2]);
    cr_assert_str_eq(ast->right->middle->values[0], "echo");
    cr_assert_str_eq(ast->right->middle->values[1], "Test");
    cr_assert_null(ast->right->middle->values[2]);
    ast_free(ast);
    lexer_free(lexer);
    reader_free(reader);
    parser_free(parser);
}

Test(parser, test_parser_nested_if)
{
    char *argv[] = { "42sh", "-c", "if echo Hello ; then if echo World ; then echo Bye ; fi ; fi" };
    struct reader *reader = reader_new(3, argv);
    struct lexer *lexer = lexer_new(reader);
    struct parser *parser = parser_new(lexer);

    struct ast *ast = parse(parser);
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, CONDITIONS);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->middle);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->middle->type, CONDITIONS);
    cr_assert_not_null(ast->middle->left);
    cr_assert_not_null(ast->middle->middle);
    cr_assert_eq(ast->middle->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->middle->middle->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0], "echo");
    cr_assert_str_eq(ast->left->values[1], "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->middle->left->values[0], "echo");
    cr_assert_str_eq(ast->middle->left->values[1], "World");
    cr_assert_null(ast->middle->left->values[2]);
    cr_assert_str_eq(ast->middle->middle->values[0], "echo");
    cr_assert_str_eq(ast->middle->middle->values[1], "Bye");
    cr_assert_null(ast->middle->middle->values[2]);
    ast_free(ast);
    lexer_free(lexer);
    reader_free(reader);
    parser_free(parser);
}