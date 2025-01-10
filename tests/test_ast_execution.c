#include <ast/ast.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <execution/execution.h>

Test(execute_node, simple_command_with_stdout)
{
    cr_redirect_stdout();

    struct ast *node = ast_new(SIMPLE_COMMAND);
    node->size = 2;
    node->values = malloc(sizeof(char *) * 3);
    node->values[0] = "echo";
    node->values[1] = "Hello, world!";
    node->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("Hello, world!\n");

    free(node->values);
}

Test(execute_node, simple_command_list)
{
    cr_redirect_stdout();

    struct ast *node = ast_new(COMMAND_LIST);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 2;
    char **value_left = malloc(sizeof(char *) * 3);
    node->left->values = value_left;
    node->left->values[0] = "echo";
    node->left->values[1] = "Hello,";
    node->left->values[2] = NULL;

    node->right = ast_new(COMMAND_LIST);
    node->right->left = ast_new(SIMPLE_COMMAND);
    node->right->left->size = 2;
    char **value_right = malloc(sizeof(char *) * 3);
    node->right->left->values = value_right;
    node->right->left->values[0] = "echo";
    node->right->left->values[1] = "world!";
    node->right->left->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("Hello,\nworld!\n");
    free(value_right);
    free(value_left);
}

Test(execute_node, last_value_count_on_command_list)
{
    struct ast *node = ast_new(COMMAND_LIST);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    char **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = "false";
    node->left->values[1] = NULL;

    node->right = ast_new(COMMAND_LIST);
    node->right->left = ast_new(SIMPLE_COMMAND);
    node->right->left->size = 1;
    char **value_right = malloc(sizeof(char *) * 2);
    node->right->left->values = value_right;
    node->right->left->values[0] = "true";
    node->right->left->values[1] = NULL;

    cr_assert_eq(execute_node(node), 0);

    free(value_right);
    free(value_left);
}

Test(execute_node, condition_node_execution)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(CONDITIONS);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    char **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = "true";
    node->left->values[1] = NULL;

    node->middle = ast_new(SIMPLE_COMMAND);
    node->middle->size = 2;
    char **value_middle = malloc(sizeof(char *) * 3);
    node->middle->values = value_middle;
    node->middle->values[0] = "echo";
    node->middle->values[1] = "Hello, world!";
    node->middle->values[2] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 1;
    char **value_right = malloc(sizeof(char *) * 2);
    node->right->values = value_right;
    node->right->values[0] = "false";
    node->right->values[1] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("Hello, world!\n");

    free(value_right);
    free(value_left);
}

Test(execute_node, condition_node_execution_false)
{
    cr_redirect_stdout();
    struct ast *node = ast_new(CONDITIONS);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    char **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = "false";
    node->left->values[1] = NULL;

    node->middle = ast_new(SIMPLE_COMMAND);
    node->middle->size = 2;
    char **value_middle = malloc(sizeof(char *) * 3);
    node->middle->values = value_middle;
    node->middle->values[0] = "echo";
    node->middle->values[1] = "Hello, world!";
    node->middle->values[2] = NULL;

    node->right = ast_new(SIMPLE_COMMAND);
    node->right->size = 3;
    char **value_right = malloc(sizeof(char *) * 4);
    node->right->values = value_right;
    node->right->values[0] = "echo";
    node->right->values[1] = "-n";
    node->right->values[2] = "Hello, world!";
    node->right->values[3] = NULL;

    cr_assert_eq(execute_node(node), 0);

    fflush(stdout);
    cr_assert_stdout_eq_str("Hello, world!");

    free(value_right);
    free(value_left);
}

Test(execute_node, condition_without_else_closure)
{
    struct ast *node = ast_new(CONDITIONS);
    node->left = ast_new(SIMPLE_COMMAND);
    node->left->size = 1;
    char **value_left = malloc(sizeof(char *) * 2);
    node->left->values = value_left;
    node->left->values[0] = "false";
    node->left->values[1] = NULL;

    node->middle = ast_new(SIMPLE_COMMAND);
    node->middle->size = 2;
    char **value_middle = malloc(sizeof(char *) * 3);
    node->middle->values = value_middle;
    node->middle->values[0] = "echo";
    node->middle->values[1] = "Hello, world!";
    node->middle->values[2] = NULL;

    cr_assert_eq(execute_node(node), 0);

    free(value_left);
    free(value_middle);
}
