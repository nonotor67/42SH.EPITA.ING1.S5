#include "../src/io/io.h"

#include <criterion/criterion.h>
#include <unistd.h>

Test(read_input, test_read_input_from_file)
{
    char *argv[] = {"./bin/42sh", "input.txt"};
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    char *result = malloc(100);
    int i = 0;
    while (1)
    {
        int c = reader_next(reader);
        if (c == EOF)
            break;
        result[i++] = c;
    }
    result[i] = '\0';
    reader_free(reader);
    char *expected = "Hello, World!\nI am a test file.\n";
    cr_assert_str_eq(result, expected, "Expected %s, but got %s", expected, result);
    free(result);
}