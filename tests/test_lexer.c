#include <criterion/criterion.h>
#include <unistd.h>

#include <io/io.h>
#include <lexer/lexer.h>

Test(lexer_simple, simple_words)
{
    char *argv[] = { "42sh", "-c", "echo Hello World" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct token token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.value, "echo");
    free(token.value);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.value, "Hello");
    free(token.value);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.value, "World");
    free(token.value);

    reader_free(reader);
    lexer_free(lexer);
}
