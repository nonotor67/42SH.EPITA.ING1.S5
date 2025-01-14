#include <criterion/criterion.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <lexer/token.h>
#include <parser/parser_struct.h>

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

#define EAT_WORD(str)                                                          \
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
    EAT_WORD("echo")
    EAT_WORD("Hello")
    EXPECT(TOKEN_SEMICOLON)
    EAT_WORD("echo")
    EAT_WORD("World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, words_newline)
{
    struct reader *reader = reader_from_string("echo Hello\n echo World");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("Hello")
    EXPECT(TOKEN_EOL)
    EAT_WORD("echo")
    EAT_WORD("World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, simple_quotes)
{
    struct reader *reader = reader_from_string("echo 'Hello World'");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("Hello World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, quotes_quotes)
{
    struct reader *reader = reader_from_string("echo 'Hello''World'");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("HelloWorld")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, escaped_quotes)
{
    struct reader *reader = reader_from_string("echo \\'Hello\\'");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("'Hello'")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, escaped_spaces)
{
    struct reader *reader = reader_from_string("echo Hello\\ World");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("Hello World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, escaped_quotes_and_spaces)
{
    struct reader *reader = reader_from_string("echo 'Hello\\ World'");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("Hello\\ World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, lexer_comments)
{
    struct reader *reader = reader_from_string("echo Hello # World");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("Hello")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, double_quoted_comment)
{
    struct reader *reader = reader_from_string("echo \"Hello # World\"");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("Hello # World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, points)
{
    struct reader *reader = reader_from_string("echo Hello. World.");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo");
    EAT_WORD("Hello.");
    EAT_WORD("World.");
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, commas)
{
    struct reader *reader = reader_from_string("echo Hello, World,");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("Hello,");
    EAT_WORD("World,");
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
    EAT_WORD("echo")
    EAT_WORD(
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
    EAT_WORD("echo")
    EAT_WORD("Hello@World")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, weird_chars2)
{
    struct reader *reader = reader_from_string("echo ^chouette %po-u*et");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("^chouette")
    EAT_WORD("%po-u*et")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, dollar_tail)
{
    struct reader *reader = reader_from_string("echo Hello$");
    struct lexer *lexer = lexer_new(reader);

    struct token token;
    EAT_WORD("echo")
    EAT_WORD("Hello$")
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
    EAT_WORD("echo")
    EAT_WORD("Hello")
    EXPECT_REDIR(">>")
    EAT_WORD("file.txt")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, redirections_number)
{
    INIT_LEXER_TEST("echo 12> file.txt")
    EAT_WORD("echo")
    EXPECT_REDIR("12>")
    EAT_WORD("file.txt")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, redirections_trap)
{
    INIT_LEXER_TEST("echo \\2> file.txt")
    EAT_WORD("echo")
    EAT_WORD("2")
    EXPECT_REDIR(">")
    EAT_WORD("file.txt")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, redirections_trap2)
{
    INIT_LEXER_TEST("echo '2'> file.txt")
    EAT_WORD("echo")
    EAT_WORD("2")
    EXPECT_REDIR(">")
    EAT_WORD("file.txt")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, redirections_double)
{
    INIT_LEXER_TEST("echo Hello >> file.txt 2>&1")
    EAT_WORD("echo")
    EAT_WORD("Hello")
    EXPECT_REDIR(">>")
    EAT_WORD("file.txt")
    EXPECT_REDIR("2>&")
    EAT_WORD("1")
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, simple_variable)
{
    INIT_LEXER_TEST("echo $HOME")
    EAT_WORD("echo")
    token = lexer_pop(lexer);
    cr_assert_str_eq(token.word->value.data, "");
    cr_assert_eq(token.word->var_length, 1);
    cr_assert_not_null(token.word->variables);
    cr_assert_str_eq(token.word->variables[0].name.data, "HOME");
    cr_assert_eq(token.word->variables[0].pos, 0);
    word_free(token.word);
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}

Test(lexer, variable_in_word_tricky)
{
    INIT_LEXER_TEST("echo '$a'$b'$c'")
    EAT_WORD("echo")
    token = lexer_pop(lexer);
    cr_assert_str_eq(token.word->value.data, "$a$c");
    cr_assert_eq(token.word->var_length, 1);
    cr_assert_not_null(token.word->variables);
    cr_assert_str_eq(token.word->variables[0].name.data, "b");
    fprintf(stderr, "pos: %zu\n", token.word->variables[0].pos);
    cr_assert_eq(token.word->variables[0].pos, 2);
    word_free(token.word);
    EXPECT(TOKEN_EOF)

    free(lexer);
    free(reader);
}
