#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <execution/execution.h>
#include <stdlib.h>
#include <unistd.h>

Test(exec_true, test_exec_true)
{
    int argc = 1;
    char *argv[] = { "true", NULL };
    cr_assert(exec_true(argc, argv) == 0, "Expected true return false");
}

Test(exec_false, test_exec_false)
{
    int argc = 1;
    char *argv[] = { "false", NULL };
    cr_assert(exec_false(argc, argv) == 1, "Expected false return true");
}

Test(exec_echo_no_args, test_exec_echo_simple)
{
    cr_redirect_stdout();

    char *argv[] = { "echo", "Hello, World!", NULL };
    exec_echo(2, argv);

    cr_assert_stdout_eq_str("Hello, World!\n");
}

Test(exec_echo_multiple, test_exec_echo_multiple)
{
    cr_redirect_stdout();

    char *argv[] = { "echo", "Hello,", "World!", NULL };
    exec_echo(3, argv);

    cr_assert_stdout_eq_str("Hello, World!\n");
}

Test(exec_echo_e, test_exec_echo_e)
{
    cr_redirect_stdout();

    char *argv[] = { "echo", "-e", "Hello,\\nWorld!", NULL };
    exec_echo(3, argv);

    cr_assert_stdout_eq_str("Hello,\nWorld!\n");
}

Test(exec_echo_n, test_exec_echo_n)
{
    cr_redirect_stdout();

    char *argv[] = { "echo", "-n", "Hello, World!", NULL };
    exec_echo(3, argv);

    cr_assert_stdout_eq_str("Hello, World!");
}

Test(exec_echo_kebab, test_exec_echo_kebab)
{
    cr_redirect_stdout();

    char *argv[] = { "echo", "-kebab", NULL };
    exec_echo(2, argv);

    cr_assert_stdout_eq_str("-kebab\n");
}

Test(exec_exit, test_exec_exit, .exit_code = 2)
{
    char *argv[] = { "exit", "2", NULL };
    exec_exit(2, argv);
}

Test(exec_exit_no_args, test_exec_exit_no_args, .exit_code = 0)
{
    char *argv[] = { "exit", NULL };
    exec_exit(1, argv);
}

Test(exec_exit_exit_code_too_high, test_exec_exit_exit_code_too_high,
     .exit_code = 255)
{
    char *argv[] = { "exit", "256", NULL };
    exec_exit(2, argv);
}

Test(exec_builtin_export, test_exec_builtin_export)
{
    char *argv[] = { "export", "HELLO=WORLD", NULL };
    exec_export(2, argv);

    printf("%s\n", getenv("HELLO"));

    cr_assert_str_eq(getenv("HELLO"), "WORLD",
                     "Expected HELLO to be set to WORLD");
}

Test(exec_builtin_export_unset, test_exec_builtin_export_unset)
{
    char *argv[] = { "export", "HELLO=WORLD", NULL };
    exec_export(2, argv);

    char *argv_unset[] = { "unset", "HELLO", NULL };
    exec_unset(2, argv_unset);

    cr_assert_null(getenv("HELLO"), "Expected HELLO to be unset");
}
