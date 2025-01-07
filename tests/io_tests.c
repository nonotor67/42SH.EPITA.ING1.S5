#include "../src/io/io.h"

#include <criterion/criterion.h>
#include <unistd.h>

Test(read_input, test_read_input_from_file)
{
    char *argv[] = {"./bin/42sh", "tests/input.txt"};
    char *result = read_input(2, argv);
    char *expected = "Hello, World!\n";
    cr_assert_str_eq(result, expected, "Expected %s, but got %s", expected, result);
}

Test(read_input, test_read_input_from_string)
{
    char *argv[] = {"./bin/42sh", "-c", "Hello, World!\n"};
    char *result = read_input(3, argv);
    char *expected = "Hello, World!\n";
    cr_assert_str_eq(result, expected, "Expected %s, but got %s", expected, result);
}

Test(read_input, test_read_input_from_stdin)
{
    char *argv[] = {"./bin/42sh"};
    char *result = read_input(1, argv);
    write(0, "Hello, World!\n", 14);
    char *expected = "Hello, World!\n";
    cr_assert_str_eq(result, expected, "Expected %s, but got %s", expected, result);
}
