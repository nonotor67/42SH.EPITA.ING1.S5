#include <ast/ast.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <execution/execution.h>

#include "utils/word.h"

struct word *my_strdup(const char *str)
{
    struct word *word = word_new();
    for (size_t i = 0; str[i]; i++)
        word_push(word, str[i]);
    return word;
}

Test(execute_node, simple_command_with_stdout)
{
    cr_redirect_stdout();

    struct ast *node = ast_new(SIMPLE_COMMAND);
    node->size = 2;
    node->values = malloc(sizeof(char *) * 3);
    node->values[0] = my_strdup("echo");
    node->values[1] = my_strdup("Hello, world!");
    node->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("Hello, world!\n");

    ast_free(node);
}

Test(execute_node, simple_command_list)
{
    cr_redirect_stdout();

    struct ast *node = ast_new(COMMAND_LIST);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 2;
    struct word **value_left = malloc(sizeof(struct word *) * 3);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("echo");
    node->left->values[1] = my_strdup("Hello,");
    node->left->values[2] = NULL;

    node->right = ast_new(COMMAND_LIST);
    node->right->left = ast_new(SIMPLE_COMMAND);
    node->right->left->size = 2;
    struct word **value_right = malloc(sizeof(struct word *) * 3);
    node->right->left->values = value_right;
    node->right->left->values[0] = my_strdup("echo");
    node->right->left->values[1] = my_strdup("world!");
    node->right->left->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("Hello,\nworld!\n");
    ast_free(node);
}

Test(execute_node, last_value_count_on_command_list)
{
    struct ast *node = ast_new(COMMAND_LIST);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("echo");
    node->left->values[1] = NULL;

    node->right = ast_new(COMMAND_LIST);
    node->right->left = ast_new(SIMPLE_COMMAND);
    node->right->left->size = 1;
    struct word **value_right = malloc(sizeof(char *) * 2);
    node->right->left->values = value_right;
    node->right->left->values[0] = my_strdup("echo");
    node->right->left->values[1] = NULL;

    cr_assert_eq(execute_node(node), 0);

    ast_free(node);
}

Test(execute_node, condition_node_execution)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(CONDITIONS);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("true");
    node->left->values[1] = NULL;

    node->middle = ast_new(SIMPLE_COMMAND);
    node->middle->size = 2;
    struct word **value_middle = malloc(sizeof(char *) * 3);
    node->middle->values = value_middle;
    node->middle->values[0] = my_strdup("echo");
    node->middle->values[1] = my_strdup("Hello, world!");
    node->middle->values[2] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 1;
    struct word **value_right = malloc(sizeof(char *) * 2);
    node->right->values = value_right;
    node->right->values[0] = my_strdup("echo");
    node->right->values[1] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("Hello, world!\n");

    ast_free(node);
}

Test(execute_node, condition_node_execution_false)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(CONDITIONS);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("false");
    node->left->values[1] = NULL;

    node->middle = ast_new(SIMPLE_COMMAND);
    node->middle->size = 2;
    struct word **value_middle = malloc(sizeof(char *) * 3);
    node->middle->values = value_middle;
    node->middle->values[0] = my_strdup("echo");
    node->middle->values[1] = my_strdup("Hello, world!");
    node->middle->values[2] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 3;
    struct word **value_right = malloc(sizeof(char *) * 4);
    node->right->values = value_right;
    node->right->values[0] = my_strdup("echo");
    node->right->values[1] = my_strdup("-n");
    node->right->values[2] = my_strdup("Hello, world!");
    node->right->values[3] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("Hello, world!");

    ast_free(node);
}

Test(execute_node, condition_without_else_closure)
{
    struct ast *node = ast_new(CONDITIONS);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("false");
    node->left->values[1] = NULL;

    node->middle = ast_new(SIMPLE_COMMAND);
    node->middle->size = 2;
    struct word **value_middle = malloc(sizeof(char *) * 3);
    node->middle->values = value_middle;
    node->middle->values[0] = my_strdup("echo");
    node->middle->values[1] = my_strdup("Hello, world!");
    node->middle->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    ast_free(node);
}

Test(execute_node, simple_pipeline)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(PIPELINE);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 2;
    struct word **value_left = malloc(sizeof(char *) * 3);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("echo");
    node->left->values[1] = my_strdup("Hello");
    node->left->values[2] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 2;
    struct word **value_right = malloc(sizeof(char *) * 2);
    node->right->values = value_right;
    node->right->values[0] = my_strdup("wc");
    node->right->values[1] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("      1       1       6\n");

    ast_free(node);
}

Test(execute_node, multiple_pipelines)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(PIPELINE);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 2;
    struct word **value_left = malloc(sizeof(char *) * 3);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("echo");
    node->left->values[1] = my_strdup("Hello");
    node->left->values[2] = NULL;

    node->right = ast_new(PIPELINE);
    node->right->left = ast_new(SIMPLE_COMMAND);
    node->right->left->size = 2;
    struct word **value_right_left = malloc(sizeof(char *) * 2);
    node->right->left->values = value_right_left;
    node->right->left->values[0] = my_strdup("cat");
    node->right->left->values[1] = NULL;

    node->right->right = ast_new(SIMPLE_COMMAND);
    node->right->right->size = 2;
    struct word **value_right_right = malloc(sizeof(char *) * 3);
    node->right->right->values = value_right_right;
    node->right->right->values[0] = my_strdup("wc");
    node->right->right->values[1] = my_strdup("-c");
    node->right->right->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("6\n");

    ast_free(node);
}

Test(execute_node, pipeline_with_builtin)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(PIPELINE);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 2;
    struct word **value_left = malloc(sizeof(char *) * 3);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("echo");
    node->left->values[1] = my_strdup("Hello, world!");
    node->left->values[2] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 2;
    struct word **value_right = malloc(sizeof(char *) * 3);
    node->right->values = value_right;
    node->right->values[0] = my_strdup("echo");
    node->right->values[1] = my_strdup("maison");
    node->right->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("maison\n");

    ast_free(node);
}

Test(execute_node, simple_or_condition)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(OR);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("false");
    node->left->values[1] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 2;
    struct word **value_right = malloc(sizeof(char *) * 3);
    node->right->values = value_right;
    node->right->values[0] = my_strdup("echo");
    node->right->values[1] = my_strdup("Hello, world!");
    node->right->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("Hello, world!\n");
    ast_free(node);
}

Test(execute_node, simple_or_lazy)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(OR);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("true");
    node->left->values[1] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 2;
    struct word **value_right = malloc(sizeof(char *) * 3);
    node->right->values = value_right;
    node->right->values[0] = my_strdup("echo");
    node->right->values[1] = my_strdup("Hello, world!");
    node->right->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("");
    ast_free(node);
}

Test(execute_node, simple_and_condition)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(AND);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("true");
    node->left->values[1] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 2;
    struct word **value_right = malloc(sizeof(char *) * 3);
    node->right->values = value_right;
    node->right->values[0] = my_strdup("echo");
    node->right->values[1] = my_strdup("Hello, world!");
    node->right->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("Hello, world!\n");
    ast_free(node);
}

Test(execute_node, simple_and_lazy)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(AND);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("false");
    node->left->values[1] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 2;
    struct word **value_right = malloc(sizeof(char *) * 3);
    node->right->values = value_right;
    node->right->values[0] = my_strdup("echo");
    node->right->values[1] = my_strdup("Hello, world!");
    node->right->values[2] = NULL;

    cr_assert_eq(execute_node(node), 1);

    fflush(stdout);
    cr_assert_stdout_eq_str("");
    ast_free(node);
}

Test(execute_node, simple_negation)
{
    struct ast *node = ast_new(NEGATION);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("false");
    node->left->values[1] = NULL;

    cr_assert_eq(execute_node(node), 0);

    ast_free(node);
}

Test(execute_node, simple_negation_true)
{
    struct ast *node = ast_new(NEGATION);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("true");
    node->left->values[1] = NULL;

    cr_assert_eq(execute_node(node), 1);

    ast_free(node);
}

Test(execute_node, simple_while)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(WHILE_LOOP);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("false");
    node->left->values[1] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 2;
    struct word **value_right = malloc(sizeof(char *) * 3);
    node->right->values = value_right;
    node->right->values[0] = my_strdup("echo");
    node->right->values[1] = my_strdup("Hello, world!");
    node->right->values[2] = NULL;

    cr_assert_eq(execution_while(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("");
    ast_free(node);
}

Test(execute_node, simple_until)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(UNTIL_LOOP);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    struct word **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = my_strdup("true");
    node->left->values[1] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 2;
    struct word **value_right = malloc(sizeof(char *) * 3);
    node->right->values = value_right;
    node->right->values[0] = my_strdup("echo");
    node->right->values[1] = my_strdup("Hello, world!");
    node->right->values[2] = NULL;

    cr_assert_eq(execution_until(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("");
    ast_free(node);
}
