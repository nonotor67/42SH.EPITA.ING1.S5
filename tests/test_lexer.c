#include <criterion/criterion.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <lexer/token.h>

Test(lexer, peek_pop)
{
    char *argv[] = { "42sh", "-c", "echo Hello World" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct token token = lexer_peek(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.value, "echo");
    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.value, "echo"); // check if pop is the same as peek
    free(token.value);

    token = lexer_peek(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.value, "Hello");
    free(token.value);
    lexer_pop(lexer);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.value, "World");
    free(token.value);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_null(token.value);

    reader_free(reader);
    lexer_free(lexer);
}

#define EXPECT(typ)                                                            \
    token = lexer_pop(lexer);                                                  \
    cr_assert_eq(token.type, typ);

#define EXPECT_WORD(str)                                                       \
    token = lexer_pop(lexer);                                                  \
    cr_assert_eq(token.type, TOKEN_WORD);                                      \
    cr_assert_str_eq(token.value, str);                                        \
    free(token.value);

Test(lexer, words_semicolon)
{
    char *argv[] = { "42sh", "-c", "echo Hello ; echo World" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello")
    EXPECT(TOKEN_SEMICOLON)
    EXPECT_WORD("echo")
    EXPECT_WORD("World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, words_newline)
{
    char *argv[] = { "42sh", "-c", "echo Hello\n echo World" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello")
    EXPECT(TOKEN_EOL)
    EXPECT_WORD("echo")
    EXPECT_WORD("World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, simple_quotes)
{
    char *argv[] = { "42sh", "-c", "echo 'Hello World'" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, escaped_quotes)
{
    char *argv[] = { "42sh", "-c", "echo \\'Hello\\'" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("'Hello'")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, escaped_spaces)
{
    char *argv[] = { "42sh", "-c", "echo Hello\\ World" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, escaped_quotes_and_spaces)
{
    char *argv[] = { "42sh", "-c", "echo 'Hello\\ World'" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello\\ World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, lexer_comments)
{
    char *argv[] = { "42sh", "-c", "echo Hello # World" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, double_quoted_comment)
{
    char *argv[] = { "42sh", "-c", "echo \"Hello # World\"" };
    struct reader *reader = reader_new(sizeof(argv) / sizeof(char *), argv);
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello # World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}
