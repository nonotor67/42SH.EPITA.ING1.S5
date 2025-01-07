#include <execution/builtins_bool.h>
#include <execution/builtins_echo.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>
#include <stdlib.h>

Test(exec_true, test_exec_true)
{
    cr_assert(exec_true(), "Expected true return false");
}

Test(exec_false, test_exec_false)
{
    cr_assert_not(exec_false(), "Expected false return true");
}

Test(exec_echo_no_args, test_exec_echo_simple)
{
    cr_redirect_stdout();

    char *argv[] = {"echo", "Hello, World!", NULL};
    exec_echo(2, argv);

    cr_assert_stdout_eq_str("Hello, World!\n");
}

Test(exec_echo_multiple, test_exec_echo_multiple)
{
    cr_redirect_stdout();

    char *argv[] = {"echo", "Hello,", "World!", NULL};
    exec_echo(3, argv);

    cr_assert_stdout_eq_str("Hello, World!\n");
}

Test(exec_echo_e, test_exec_echo_e)
{
    cr_redirect_stdout();

    char *argv[] = {"echo", "-e", "Hello,\\nWorld!", NULL};
    exec_echo(3, argv);

    cr_assert_stdout_eq_str("Hello,\nWorld!\n");
}

Test(exec_echo_n, test_exec_echo_n)
{
    cr_redirect_stdout();

    char *argv[] = {"echo", "-n", "Hello, World!", NULL};
    exec_echo(3, argv);

    cr_assert_stdout_eq_str("Hello, World!");
}

Test(exec_echo_kebab, test_exec_echo_kebab)
{
    cr_redirect_stdout();

    char *argv[] = {"echo", "-kebab", NULL};
    exec_echo(2, argv);

    cr_assert_stdout_eq_str("-kebab\n");
}