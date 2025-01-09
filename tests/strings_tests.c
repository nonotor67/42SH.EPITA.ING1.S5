#include <criterion/criterion.h>
#include <unistd.h>

#include <utils/strings.h>
#include "../src/utils/strings.h"

Test(strings, test_string_push)
{
    struct string *string = string_new();
    cr_assert_not_null(string, "Expected a non-null string, but got NULL");

    string_push(string, 'H');
    string_push(string, 'e');
    string_push(string, 'l');
    string_push(string, 'l');
    string_push(string, 'o');
    string_push(string, ',');
    string_push(string, ' ');
    string_push(string, 'W');
    string_push(string, 'o');
    string_push(string, 'r');
    string_push(string, 'l');
    string_push(string, 'd');
    string_push(string, '!');
    string_push(string, '\n');

    cr_assert_str_eq(string->data, "Hello, World!\n",
                     "Expected Hello, World!\n, but got %s", string->data);

    string_free(string);
}

Test(strings, test_string_append)
{
    struct string *string = string_new();
    cr_assert_not_null(string, "Expected a non-null string, but got NULL");

    string_append(string, "Hello, ");
    string_append(string, "World! This string has to be a bit longer...");
    string_append(string, "\n");

    cr_assert_str_eq(string->data, "Hello, World!\n",
                     "Expected Hello, World!\n, but got %s", string->data);

    string_free(string);
}

Test(strings, test_string_from)
{
    struct string *string = string_from("Hello, World!\n");
    cr_assert_not_null(string, "Expected a non-null string, but got NULL");

    cr_assert_str_eq(string->data, "Hello, World!\n",
                     "Expected Hello, World!\n, but got %s", string->data);

    string_free(string);
}

Test(strings, test_string_from_empty)
{
    struct string *string = string_from("");
    cr_assert_not_null(string, "Expected a non-null string, but got NULL");

    cr_assert_str_eq(string->data, "", "Expected an empty string, but got %s",
                     string->data);

    string_free(string);
}
