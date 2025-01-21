#include <ast/ast.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <unistd.h>

#define INIT_PARSER(str)                                                       \
    struct reader *reader = reader_from_string(str);                           \
    struct lexer *lexer = lexer_new(reader);                                   \
    struct parser *parser = parser_new(lexer);                                 \
    struct ast *ast = parse(parser);

#define CLEAR_ALL                                                              \
    ast_free(ast);                                                             \
    lexer_free(lexer);                                                         \
    reader_free(reader);                                                       \
    parser_free(parser);

Test(parser, test_parser_simple_command)
{
    INIT_PARSER("echo Hello World")

    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, SIMPLE_COMMAND);
    cr_assert_not_null(ast->values);
    cr_assert_str_eq(ast->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->values[1]->value.data, "Hello");
    cr_assert_str_eq(ast->values[2]->value.data, "World");
    cr_assert_null(ast->values[3]);
    CLEAR_ALL
}

Test(parser, test_parser_command_list)
{
    INIT_PARSER("echo Hello ; echo World")

    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, COMMAND_LIST);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, COMMAND_LIST);
    cr_assert_eq(ast->right->left->type, SIMPLE_COMMAND);

    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->left->values[1]->value.data, "World");
    cr_assert_null(ast->right->left->values[2]);
    CLEAR_ALL
}

Test(parser, test_parser_if)
{
    INIT_PARSER("if echo Hello ; then echo World ; fi")

    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, CONDITIONS);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->middle);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->middle->type, SIMPLE_COMMAND);
    cr_assert_null(ast->right);
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->middle->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->middle->values[1]->value.data, "World");
    cr_assert_null(ast->middle->values[2]);
    CLEAR_ALL
}

Test(parser, test_parser_if_else)
{
    INIT_PARSER("if echo Hello ; then echo World ; else echo Bye ; fi")

    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, CONDITIONS);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->middle);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->middle->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->middle->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->middle->values[1]->value.data, "World");
    cr_assert_null(ast->middle->values[2]);
    cr_assert_str_eq(ast->right->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->values[1]->value.data, "Bye");
    cr_assert_null(ast->right->values[2]);
    CLEAR_ALL
}

Test(parser, test_parser_if_elif)
{
    INIT_PARSER(
        "if echo Hello ; then echo World ; elif echo Bye ; then echo Test ; fi")

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
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->middle->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->middle->values[1]->value.data, "World");
    cr_assert_null(ast->middle->values[2]);
    cr_assert_str_eq(ast->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->left->values[1]->value.data, "Bye");
    cr_assert_null(ast->right->left->values[2]);
    cr_assert_str_eq(ast->right->middle->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->middle->values[1]->value.data, "Test");
    cr_assert_null(ast->right->middle->values[2]);
    CLEAR_ALL
}

Test(parser, test_parser_nested_if)
{
    INIT_PARSER("if echo Hello ; then if echo World ; then echo Bye ; fi ; fi")

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
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->middle->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->middle->left->values[1]->value.data, "World");
    cr_assert_null(ast->middle->left->values[2]);
    cr_assert_str_eq(ast->middle->middle->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->middle->middle->values[1]->value.data, "Bye");
    cr_assert_null(ast->middle->middle->values[2]);
    CLEAR_ALL
}

Test(parser, test_redir_simple)
{
    INIT_PARSER("echo Hello > file")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, SIMPLE_COMMAND);
    cr_assert_not_null(ast->values);
    cr_assert_str_eq(ast->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->values[1]->value.data, "Hello");
    cr_assert_str_eq(ast->redir[0]->value.data, ">");
    cr_assert_str_eq(ast->redir[1]->value.data, "file");
    cr_assert_null(ast->values[2]);
    cr_assert_null(ast->redir[2]);
    CLEAR_ALL
}

Test(parser, test_redir_prefix_before)
{
    INIT_PARSER("> file echo Hello")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, SIMPLE_COMMAND);
    cr_assert_not_null(ast->values);
    cr_assert_str_eq(ast->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->values[1]->value.data, "Hello");
    cr_assert_str_eq(ast->redir[0]->value.data, ">");
    cr_assert_str_eq(ast->redir[1]->value.data, "file");
    cr_assert_null(ast->values[2]);
    cr_assert_null(ast->redir[2]);
    CLEAR_ALL
}

Test(parser, test_redir_prefix_before_and_after)
{
    INIT_PARSER("> file echo Hello 0> file.txt")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, SIMPLE_COMMAND);
    cr_assert_not_null(ast->values);
    cr_assert_str_eq(ast->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->values[1]->value.data, "Hello");
    cr_assert_str_eq(ast->redir[0]->value.data, ">");
    cr_assert_str_eq(ast->redir[1]->value.data, "file");
    cr_assert_str_eq(ast->redir[2]->value.data, "0>");
    cr_assert_str_eq(ast->redir[3]->value.data, "file.txt");
    cr_assert_null(ast->values[2]);
    cr_assert_null(ast->redir[4]);
    CLEAR_ALL
}

Test(parser, test_redir_lots)
{
    INIT_PARSER("echo Hello > file 0> file.txt 15<< EOF <&"
                " a.txt < b.txt >> c.txt 2>> d.txt >| e.txt 2>& ok <> f.txt")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, SIMPLE_COMMAND);
    cr_assert_not_null(ast->values);
    cr_assert_str_eq(ast->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->values[1]->value.data, "Hello");
    cr_assert_str_eq(ast->redir[0]->value.data, ">");
    cr_assert_str_eq(ast->redir[1]->value.data, "file");
    cr_assert_str_eq(ast->redir[2]->value.data, "0>");
    cr_assert_str_eq(ast->redir[3]->value.data, "file.txt");
    cr_assert_str_eq(ast->redir[4]->value.data, "15<<");
    cr_assert_str_eq(ast->redir[5]->value.data, "EOF");
    cr_assert_str_eq(ast->redir[6]->value.data, "<&");
    cr_assert_str_eq(ast->redir[7]->value.data, "a.txt");
    cr_assert_str_eq(ast->redir[8]->value.data, "<");
    cr_assert_str_eq(ast->redir[9]->value.data, "b.txt");
    cr_assert_str_eq(ast->redir[10]->value.data, ">>");
    cr_assert_str_eq(ast->redir[11]->value.data, "c.txt");
    cr_assert_str_eq(ast->redir[12]->value.data, "2>>");
    cr_assert_str_eq(ast->redir[13]->value.data, "d.txt");
    cr_assert_str_eq(ast->redir[14]->value.data, ">|");
    cr_assert_str_eq(ast->redir[15]->value.data, "e.txt");
    cr_assert_str_eq(ast->redir[16]->value.data, "2>&");
    cr_assert_str_eq(ast->redir[17]->value.data, "ok");
    cr_assert_str_eq(ast->redir[18]->value.data, "<>");
    cr_assert_str_eq(ast->redir[19]->value.data, "f.txt");
    cr_assert_null(ast->values[2]);
    cr_assert_null(ast->redir[20]);
    CLEAR_ALL
}

Test(parser, test_redir_tricky)
{
    INIT_PARSER("echo Hello > file test > file2")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, SIMPLE_COMMAND);
    cr_assert_not_null(ast->values);
    cr_assert_str_eq(ast->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->values[1]->value.data, "Hello");
    cr_assert_str_eq(ast->values[2]->value.data, "test");
    cr_assert_str_eq(ast->redir[0]->value.data, ">");
    cr_assert_str_eq(ast->redir[1]->value.data, "file");
    cr_assert_str_eq(ast->redir[2]->value.data, ">");
    cr_assert_str_eq(ast->redir[3]->value.data, "file2");
    cr_assert_null(ast->values[3]);
    cr_assert_null(ast->redir[4]);
    CLEAR_ALL
}

Test(parser, test_redir_only)
{
    INIT_PARSER("> file < file2")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, SIMPLE_COMMAND);
    cr_assert_not_null(ast->values);
    cr_assert_null(ast->values[0]);
    cr_assert_str_eq(ast->redir[0]->value.data, ">");
    cr_assert_str_eq(ast->redir[1]->value.data, "file");
    cr_assert_str_eq(ast->redir[2]->value.data, "<");
    cr_assert_str_eq(ast->redir[3]->value.data, "file2");
    cr_assert_null(ast->redir[4]);
    CLEAR_ALL
}

Test(parser, test_redir_if)
{
    INIT_PARSER("if echo Hello ; then echo World; fi > file")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, CONDITIONS);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->middle);
    cr_assert_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->middle->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->middle->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->middle->values[1]->value.data, "World");
    cr_assert_null(ast->middle->values[2]);
    cr_assert_str_eq(ast->redir[0]->value.data, ">");
    cr_assert_str_eq(ast->redir[1]->value.data, "file");
    cr_assert_null(ast->redir[2]);
    CLEAR_ALL
}

Test(parser, test_redir_nested_if)
{
    INIT_PARSER("if echo Hello ; then echo World > file2; fi > file")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, CONDITIONS);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->middle);
    cr_assert_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->middle->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->middle->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->middle->values[1]->value.data, "World");
    cr_assert_null(ast->middle->values[2]);
    cr_assert_str_eq(ast->redir[0]->value.data, ">");
    cr_assert_str_eq(ast->redir[1]->value.data, "file");
    cr_assert_null(ast->redir[2]);
    cr_assert_str_eq(ast->middle->redir[0]->value.data, ">");
    cr_assert_str_eq(ast->middle->redir[1]->value.data, "file2");
    cr_assert_null(ast->middle->redir[2]);
    CLEAR_ALL
}

Test(parser, test_and)
{
    INIT_PARSER("echo Hello && echo World")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, AND);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->right->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->values[1]->value.data, "World");
    cr_assert_null(ast->right->values[2]);
    CLEAR_ALL
}

Test(parser, simple_for_loop)
{
    INIT_PARSER("for i in 1 2 3 ; do echo coucou ; done")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, FOR_LOOP);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->middle);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, WORD_COMPONENTS);
    cr_assert_eq(ast->middle->type, WORD_COMPONENTS);
    cr_assert_eq(ast->right->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "i");
    cr_assert_null(ast->left->values[1]);
    cr_assert_str_eq(ast->middle->values[0]->value.data, "1");
    cr_assert_str_eq(ast->middle->values[1]->value.data, "2");
    cr_assert_str_eq(ast->middle->values[2]->value.data, "3");
    cr_assert_null(ast->middle->values[3]);
    cr_assert_str_eq(ast->right->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->values[1]->value.data, "coucou");
    cr_assert_null(ast->right->values[2]);
    CLEAR_ALL
}

Test(parser, test_or)
{
    INIT_PARSER("echo Hello || echo World")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, OR);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->right->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->values[1]->value.data, "World");
    cr_assert_null(ast->right->values[2]);
    CLEAR_ALL
}

Test(parser, test_parser_pipe)
{
    INIT_PARSER("echo Hello | echo World")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, PIPELINE);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, PIPELINE);
    cr_assert_eq(ast->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->left->values[1]->value.data, "World");
    cr_assert_null(ast->right->left->values[2]);
    cr_assert_null(ast->right->right);
    CLEAR_ALL
}

Test(parser, test_parser_pipe_multiple)
{
    INIT_PARSER("echo Hello | echo World | echo Test")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, PIPELINE);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, PIPELINE);
    cr_assert_eq(ast->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->left->values[1]->value.data, "World");
    cr_assert_null(ast->right->left->values[2]);
    cr_assert_eq(ast->right->right->type, PIPELINE);
    cr_assert_eq(ast->right->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->right->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->right->left->values[1]->value.data, "Test");
    cr_assert_null(ast->right->right->left->values[2]);
    cr_assert_null(ast->right->right->right);
    CLEAR_ALL
}

Test(parser, test_mix_pipeline_and_or)
{
    INIT_PARSER("echo Hello | echo World && echo Test")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, AND);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, PIPELINE);
    cr_assert_eq(ast->right->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->right->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->values[1]->value.data, "Test");
    cr_assert_null(ast->right->values[2]);
    cr_assert_eq(ast->left->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->left->values[2]);
    cr_assert_eq(ast->left->right->type, PIPELINE);
    cr_assert_eq(ast->left->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->right->left->values[1]->value.data, "World");
    cr_assert_null(ast->left->right->left->values[2]);
    cr_assert_null(ast->left->right->right);
    CLEAR_ALL
}

Test(parser, test_pipe_negation)
{
    INIT_PARSER("! echo Hello | echo World")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, NEGATION);
    cr_assert_not_null(ast->left);
    cr_assert_eq(ast->left->type, PIPELINE);
    cr_assert_eq(ast->left->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->left->right->type, PIPELINE);
    cr_assert_str_eq(ast->left->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->left->values[2]);
    cr_assert_eq(ast->left->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->right->left->values[1]->value.data, "World");
    cr_assert_null(ast->left->right->left->values[2]);
    cr_assert_null(ast->left->right->right);
    CLEAR_ALL
}

Test(parser, for_loop_eol_land)
{
    INIT_PARSER("for word\n\n\nin coucou hello hallo "
                "gutentag\n\n\n\ndo\n\n\necho coucou\n\n\n\ndone")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, FOR_LOOP);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->middle);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, WORD_COMPONENTS);
    cr_assert_eq(ast->middle->type, WORD_COMPONENTS);
    cr_assert_eq(ast->right->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "word");
    cr_assert_null(ast->left->values[1]);
    cr_assert_str_eq(ast->middle->values[0]->value.data, "coucou");
    cr_assert_str_eq(ast->middle->values[1]->value.data, "hello");
    cr_assert_str_eq(ast->middle->values[2]->value.data, "hallo");
    cr_assert_str_eq(ast->middle->values[3]->value.data, "gutentag");
    cr_assert_null(ast->middle->values[4]);
    cr_assert_str_eq(ast->right->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->values[1]->value.data, "coucou");
    cr_assert_null(ast->right->values[2]);
    CLEAR_ALL
}

Test(parser, while_loop_simple)
{
    INIT_PARSER("while echo Hello ; do echo World ; done")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, WHILE_LOOP);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->right->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->values[1]->value.data, "World");
    cr_assert_null(ast->right->values[2]);
    CLEAR_ALL
}

Test(parser, while_loop_double)
{
    INIT_PARSER("while echo Hello ; echo encore ; do echo World ; test ; done")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, WHILE_LOOP);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, COMMAND_LIST);
    cr_assert_eq(ast->left->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->left->values[2]);
    cr_assert_eq(ast->left->right->type, COMMAND_LIST);
    cr_assert_eq(ast->left->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->right->left->values[1]->value.data, "encore");
    cr_assert_null(ast->left->right->left->values[2]);
    cr_assert_eq(ast->right->type, COMMAND_LIST);
    cr_assert_eq(ast->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->left->values[1]->value.data, "World");
    cr_assert_null(ast->right->left->values[2]);
    cr_assert_eq(ast->right->right->type, COMMAND_LIST);
    cr_assert_eq(ast->right->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->right->right->left->values[0]->value.data, "test");
    cr_assert_null(ast->right->right->left->values[1]);
    CLEAR_ALL
}

Test(parser, until_loop_simple)
{
    INIT_PARSER("until echo Hello ; do echo World ; done")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, UNTIL_LOOP);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->right->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->values[2]);
    cr_assert_str_eq(ast->right->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->values[1]->value.data, "World");
    cr_assert_null(ast->right->values[2]);
    CLEAR_ALL
}

Test(parser, until_loop_double)
{
    INIT_PARSER("until echo Hello ; echo encore ; do echo World ; test ; done")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, UNTIL_LOOP);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, COMMAND_LIST);
    cr_assert_eq(ast->left->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->left->values[2]);
    cr_assert_eq(ast->left->right->type, COMMAND_LIST);
    cr_assert_eq(ast->left->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->right->left->values[1]->value.data, "encore");
    cr_assert_null(ast->left->right->left->values[2]);
    cr_assert_eq(ast->right->type, COMMAND_LIST);
    cr_assert_eq(ast->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->left->values[1]->value.data, "World");
    cr_assert_null(ast->right->left->values[2]);
    cr_assert_eq(ast->right->right->type, COMMAND_LIST);
    cr_assert_eq(ast->right->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->right->right->left->values[0]->value.data, "test");
    cr_assert_null(ast->right->right->left->values[1]);
    CLEAR_ALL
}

Test(parser, command_block_simple)
{
    INIT_PARSER("{ echo Hello ; echo World ; }")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, COMMAND_BLOCK);
    cr_assert_not_null(ast->left);
    cr_assert_eq(ast->left->type, COMMAND_LIST);
    cr_assert_not_null(ast->left->left);
    cr_assert_not_null(ast->left->right);
    cr_assert_eq(ast->left->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->left->right->type, COMMAND_LIST);
    cr_assert_str_eq(ast->left->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->left->values[2]);
    cr_assert_eq(ast->left->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->right->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->right->left->values[1]->value.data, "World");
    cr_assert_null(ast->left->right->left->values[2]);
    cr_assert_null(ast->left->right->right);
    CLEAR_ALL
}

Test(parser, command_block_multiple)
{
    INIT_PARSER("{ echo Hello ; echo World ; } ; { echo Test ;}")
    cr_assert_not_null(ast);
    cr_assert_eq(ast->type, COMMAND_LIST);
    cr_assert_not_null(ast->left);
    cr_assert_not_null(ast->right);
    cr_assert_eq(ast->left->type, COMMAND_BLOCK);
    cr_assert_eq(ast->right->type, COMMAND_LIST);
    cr_assert_not_null(ast->left->left);
    cr_assert_not_null(ast->left->left->left);
    cr_assert_not_null(ast->left->left->right);
    cr_assert_eq(ast->left->left->left->type, SIMPLE_COMMAND);
    cr_assert_eq(ast->left->left->right->type, COMMAND_LIST);
    cr_assert_str_eq(ast->left->left->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->left->left->left->values[1]->value.data, "Hello");
    cr_assert_null(ast->left->left->left->values[2]);
    cr_assert_eq(ast->left->left->right->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->left->left->right->left->values[0]->value.data,
                     "echo");
    cr_assert_str_eq(ast->left->left->right->left->values[1]->value.data,
                     "World");
    cr_assert_null(ast->left->left->right->left->values[2]);
    cr_assert_null(ast->left->left->right->right);
    cr_assert_eq(ast->right->left->type, COMMAND_BLOCK);
    cr_assert_not_null(ast->right->left->left);
    cr_assert_eq(ast->right->left->left->type, SIMPLE_COMMAND);
    cr_assert_str_eq(ast->right->left->left->values[0]->value.data, "echo");
    cr_assert_str_eq(ast->right->left->left->values[1]->value.data, "Test");
    cr_assert_null(ast->right->left->left->values[2]);
    CLEAR_ALL
}
