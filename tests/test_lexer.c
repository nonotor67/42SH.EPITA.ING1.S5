#include <criterion/criterion.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <lexer/token.h>

Test(lexer, peek_pop)
{
    struct reader *reader = reader_from_string("echo Hello World");
    struct lexer *lexer = lexer_new(reader);

    struct token token = lexer_peek(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.word->value.data, "echo");
    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.word->value.data,
                     "echo"); // check if pop is the same as peek
    word_free(token.word);

    token = lexer_peek(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.word->value.data, "Hello");
    token = lexer_pop(lexer);
    word_free(token.word);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_WORD);
    cr_assert_str_eq(token.word->value.data, "World");
    word_free(token.word);

    token = lexer_pop(lexer);
    cr_assert_eq(token.type, TOKEN_EOF);
    cr_assert_null(token.word);

    reader_free(reader);
    lexer_free(lexer);
}

#define EXPECT(typ)                                                            \
    token = lexer_pop(lexer);                                                  \
    cr_assert_eq(token.type, typ);

#define EXPECT_WORD(str)                                                       \
    token = lexer_pop(lexer);                                                  \
    cr_assert_eq(token.type, TOKEN_WORD);                                      \
    cr_assert_str_eq(token.word->value.data, str);                             \
    word_free(token.word);

#define EXPECT_REDIR(str)                                                      \
    token = lexer_pop(lexer);                                                  \
    cr_assert_eq(token.type, TOKEN_REDIR);                                     \
    cr_assert_str_eq(token.word->value.data, str);                             \
    word_free(token.word);

Test(lexer, words_semicolon)
{
    struct reader *reader = reader_from_string("echo Hello ; echo World");
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
    struct reader *reader = reader_from_string("echo Hello\n echo World");
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
    struct reader *reader = reader_from_string("echo 'Hello World'");
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
    struct reader *reader = reader_from_string("echo \\'Hello\\'");
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
    struct reader *reader = reader_from_string("echo Hello\\ World");
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
    struct reader *reader = reader_from_string("echo 'Hello\\ World'");
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
    struct reader *reader = reader_from_string("echo Hello # World");
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
    struct reader *reader = reader_from_string("echo \"Hello # World\"");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello # World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, points)
{
    struct reader *reader = reader_from_string("echo Hello. World.");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo");
    EXPECT_WORD("Hello.");
    EXPECT_WORD("World.");
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, commas)
{
    struct reader *reader = reader_from_string("echo Hello, World,");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello,");
    EXPECT_WORD("World,");
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, very_long)
{
    struct reader *reader = reader_from_string(
        "echo Loremipsumdolorsitamet,consecteturadipiscingelit."
        "Aliquamconsequatmassasedurnavenenatisbibendum.Praesentavariusenim,"
        "necvehiculanulla.Utatlectusaliquam,consecteturmagnased,suscipitipsum."
        "Maurisnecmaurisex.Utsednullasuscipit,");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD(
        "Loremipsumdolorsitamet,consecteturadipiscingelit."
        "Aliquamconsequatmassasedurnavenenatisbibendum.Praesentavariusenim,"
        "necvehiculanulla.Utatlectusaliquam,consecteturmagnased,suscipitipsum."
        "Maurisnecmaurisex.Utsednullasuscipit,");
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, weird_chars)
{
    struct reader *reader = reader_from_string("echo Hello@World");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello@World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, weird_chars2)
{
    struct reader *reader = reader_from_string("echo ^chouette %po-u*et");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("^chouette")
    EXPECT_WORD("%po-u*et")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, dollar_tail)
{
    struct reader *reader = reader_from_string("echo Hello$");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello$")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

#define INIT_LEXER_TEST(str)                                                   \
    struct reader *reader = reader_from_string(str);                           \
    struct lexer *lexer = lexer_new(reader);                                   \
    struct token token;

Test(lexer, redirections_simple)
{
    INIT_LEXER_TEST("echo Hello >> file.txt")
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello")
    EXPECT_REDIR(">>")
    EXPECT_WORD("file.txt")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, redirections_number)
{
    INIT_LEXER_TEST("echo 12> file.txt")
    EXPECT_WORD("echo")
    EXPECT_REDIR("12>")
    EXPECT_WORD("file.txt")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, redirections_trap)
{
    INIT_LEXER_TEST("echo \\2> file.txt")
    EXPECT_WORD("echo")
    EXPECT_WORD("2")
    EXPECT_REDIR(">")
    EXPECT_WORD("file.txt")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, redirections_trap2)
{
    INIT_LEXER_TEST("echo '2'> file.txt")
    EXPECT_WORD("echo")
    EXPECT_WORD("2")
    EXPECT_REDIR(">")
    EXPECT_WORD("file.txt")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, redirections_double)
{
    INIT_LEXER_TEST("echo Hello >> file.txt 2>&1")
    EXPECT_WORD("echo")
    EXPECT_WORD("Hello")
    EXPECT_REDIR(">>")
    EXPECT_WORD("file.txt")
    EXPECT_REDIR("2>&")
    EXPECT_WORD("1")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}
