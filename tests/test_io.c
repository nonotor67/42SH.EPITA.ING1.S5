#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <io/io.h>
#include <unistd.h>

Test(read_input, test_read_input_from_file)
{
    char *argv[] = { "./bin/42sh", "input.txt" };
    char *result = read_input(2, argv);
    char *expected = "Hello, World!\n";
    cr_assert_str_eq(result, expected, "Expected %s, but got %s", expected,
                     result);
    free(result);
}

Test(read_input, test_read_input_from_string)
{
    char *argv[] = { "./bin/42sh", "-c", "Hello, World!\n" };
    char *result = read_input(3, argv);
    char *expected = "Hello, World!\n";
    cr_assert_str_eq(result, expected, "Expected %s, but got %s", expected,
                     result);
    free(result);
}

/*
Test(read_from_stdin, valid_input)
{
    cr_redirect_stdin();

    const char *input_data = "Hello, world!\nThis is a test.\n";
    fputs(input_data, stdin);

    rewind(stdin);

    char *result = read_from_stdin();

    cr_assert_not_null(result, "La fonction doit retourner une chaîne valide.");
    cr_assert_str_eq(result, input_data, "La sortie attendue est identique à
l'entrée.");

    free(result);
} */