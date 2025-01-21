#include "lexer_internal.h"

#include <ctype.h>
#include <io/io.h>
#include <string.h>
#include <utils/utils.h>

#include "lexer.h"

static int last_char(struct lexer *lexer)
{
    if (lexer->current_char == UNITIALIZED_CHAR) // read the next character
        lexer->current_char = reader_next(lexer->reader);
    return lexer->current_char;
}

static int next_char(struct lexer *lexer)
{
    lexer->current_char = reader_next(lexer->reader);
    return lexer->current_char;
}

// Skip the whitespace characters in the data
static void lexer_skip_whitespace(struct lexer *lexer)
{
    while (isspace(last_char(lexer)) && last_char(lexer) != '\n')
        next_char(lexer);
}

static void lexer_skip_comment(struct lexer *lexer)
{
    while (lexer->mode == LEXING_COMMENT && last_char(lexer) != '\n'
           && last_char(lexer) != EOF)
        next_char(lexer);
    lexer->mode = LEXING_NORMAL;
}

// Test if a character is a word character
static int lexer_is_word_char(struct lexer *lexer)
{
    const int c = last_char(lexer);
    if (c == EOF)
        return 0;
    static const char reserved[] = " \t\n;|&<>!()";
    for (size_t i = 0; i < sizeof(reserved) - 1; i++)
        if (c == reserved[i])
            return 0;
    return 1;
}

static int lexer_is_name_char(struct lexer *lexer)
{
    const int c = last_char(lexer);
    return isalnum(c) || c == '_';
}

// Returns true if it is a redirection and adds the redirection to the word
static int lexer_try_redir(struct lexer *lexer, struct word *word)
{
    char c = (char)last_char(lexer);
    if (c != '>' && c != '<')
        return 0;
    // check if word is a number
    for (size_t i = 0; i < word->value.length; i++)
        if (!isdigit(word->value.data[i]))
            return 0;

    string_push(&word->value, c);
    next_char(lexer);

    // check if the next character completes the redirection
    char c2 = (char)last_char(lexer);
    char str[] = { c, c2, '\0' };
    if (strcmp(str, ">>") == 0 || strcmp(str, "<<") == 0
        || strcmp(str, "<>") == 0 || strcmp(str, ">&") == 0
        || strcmp(str, "<&") == 0 || strcmp(str, ">|") == 0)
    {
        string_push(&word->value, c2);
        next_char(lexer);
    }

    return 1;
}

// returns true and populates var if it parsed a variable, false otherwise
static int lexer_lex_variable(struct lexer *lexer, struct variable *var)
{
    if (last_char(lexer) == EOF)
        return 0;
    static const char accepted_single[] = "$*!@#?-";
    const char c = (char)last_char(lexer);
    // try special vars first
    for (size_t i = 0; i < sizeof(accepted_single) - 1; i++)
    {
        if (c == accepted_single[i])
        {
            string_init(&var->name);
            string_push(&var->name, c);
            next_char(lexer);
            return 1;
        }
    }
    const int bracket = c == '{';
    if (bracket)
        next_char(lexer);

    // try normal vars
    if (!bracket && !lexer_is_name_char(lexer))
        return 0;

    string_init(&var->name);
    while (lexer_is_name_char(lexer))
    {
        string_push(&var->name, (char)last_char(lexer));
        next_char(lexer);
    }
    if (bracket)
    {
        if (last_char(lexer) != '}')
            var->name.length = 0;
        else
            next_char(lexer);
    }
    return 1;
}

static int lexer_try_variable(struct lexer *lexer, struct word *word)
{
    char c = (char)last_char(lexer);
    if (c != '$')
        return 0;

    next_char(lexer);
    struct variable var;

    size_t length = word->value.length;
    if (length > 0 && word->value.data[length - 1] == '\\')
        // replace the backslash with '$'
        word->value.data[length - 1] = '$';
    else if (lexer_lex_variable(lexer, &var))
    {
        var.pos = word->value.length;
        word_push_variable(word, var);
    }
    else
        string_push(&word->value, c);
    return 1;
}

static int open_quoting(struct lexer *lexer, char c)
{
    if (c == '\'')
        lexer->mode = LEXING_QUOTED;
    else if (c == '"')
        lexer->mode = LEXING_DOUBLE_QUOTED;
    else
        return 0;
    lexer->current_char = UNITIALIZED_CHAR;
    return 1;
}

static int close_quoting(struct lexer *lexer, char c)
{
    if ((lexer->mode == LEXING_QUOTED && c == '\'')
        || (c == '"' && lexer->mode == LEXING_DOUBLE_QUOTED))
        lexer->mode = LEXING_NORMAL;
    else
        return 0;
    lexer->current_char = UNITIALIZED_CHAR;
    return 1;
}

static int handle_escape(struct lexer *lexer)
{
    if (last_char(lexer) != '\\')
        return 0;
    lexer->escape_next = 1;
    lexer->current_char = UNITIALIZED_CHAR;
    return 1;
}

#define BLIND_PUSH_WHEN(cond)                                                  \
    if (cond)                                                                  \
    {                                                                          \
        string_push(&word->value, last_char(lexer));                           \
        next_char(lexer);                                                      \
        lexer->escape_next = 0;                                                \
        continue;                                                              \
    }

#define SKIP_WHEN(cond)                                                        \
    if (cond)                                                                  \
        continue;

#define IS_ESCAPE_STATE                                                        \
    (lexer->escape_next || lexer->mode == LEXING_QUOTED                        \
     || lexer->mode == LEXING_DOUBLE_QUOTED)
// Create a word token or a keyword token
static struct token lexer_next_handle_word(struct lexer *lexer)
{
    struct word *word = word_new();
    int met_equal = 0;
    while (lexer_is_word_char(lexer) || IS_ESCAPE_STATE)
    {
        // has_escaped can only go from 0 to 1 and never back to 0 thanks to |=
        word->has_escaped |= IS_ESCAPE_STATE;
        word->valid_assignment &= met_equal || !word->has_escaped;
        char c = (char)last_char(lexer);

        if (c == '=')
            met_equal = 1;
        BLIND_PUSH_WHEN(lexer->escape_next);
        SKIP_WHEN(close_quoting(lexer, c));
        BLIND_PUSH_WHEN(lexer->mode == LEXING_QUOTED);
        SKIP_WHEN(lexer_try_variable(lexer, word));
        BLIND_PUSH_WHEN(lexer->mode == LEXING_DOUBLE_QUOTED);
        SKIP_WHEN(open_quoting(lexer, c));
        SKIP_WHEN(handle_escape(lexer));
        BLIND_PUSH_WHEN(1);
    }
    struct token token;
    token.type = TOKEN_WORD;

    if (!word->has_escaped && lexer_try_redir(lexer, word))
        token.type = TOKEN_REDIR;

    token.word = word;
    return token;
}

static struct token lexer_switch(struct lexer *lexer)
{
    struct token token;
    token.word = NULL;
    switch (last_char(lexer))
    {
    case '\n':
        token.type = TOKEN_EOL;
        break;
    case ';':
        token.type = TOKEN_SEMICOLON;
        break;
    case '(':
        token.type = TOKEN_LEFT_PAREN;
        break;
    case ')':
        token.type = TOKEN_RIGHT_PAREN;
        break;
    case '|':
        token.type = next_char(lexer) == '|' ? TOKEN_OR : TOKEN_PIPE;
        break;
    case '&':
        token.type = next_char(lexer) == '&' ? TOKEN_AND : TOKEN_UNKNOWN;
        break;
    case '!':
        token.type = TOKEN_NEGATION;
        break;
    case '\'':
        lexer->mode = LEXING_QUOTED;
        lexer->current_char = UNITIALIZED_CHAR;
        return lexer_next_handle_word(lexer);
    case '"':
        lexer->mode = LEXING_DOUBLE_QUOTED;
        lexer->current_char = UNITIALIZED_CHAR;
        return lexer_next_handle_word(lexer);
    case '#':
        lexer->mode = LEXING_COMMENT;
        lexer->current_char = UNITIALIZED_CHAR;
        return lexer_next(lexer);
    case '>':
    case '<':
        // redirections are handled in lexer_next_handle_word
        return lexer_next_handle_word(lexer);
    default:
        token.type = TOKEN_UNKNOWN;
        break;
    }
    if (token.type != TOKEN_PIPE)
        lexer->current_char = UNITIALIZED_CHAR;
    return token;
}

static void lex_keyword_or_word(struct lexer *lexer, struct token *token)
{
    lexer->context = LEXING_ARGUMENT;
    if (token->word->has_escaped)
        return;
    static const char *reserved_keywords[] = { "if",   "then",  "else",  "elif",
                                               "fi",   "do",    "done",  "case",
                                               "esac", "while", "until", "for",
                                               "in",   NULL };
    for (size_t i = 0; reserved_keywords[i] && token->type == TOKEN_WORD; i++)
        if (strcmp(token->word->value.data, reserved_keywords[i]) == 0)
            token->type = TOKEN_KEYWORD;
}

#define TOKEN_IS_CLOSER(tok)                                                   \
    (tok.type != TOKEN_WORD && tok.type != TOKEN_KEYWORD)

struct token lexer_next(struct lexer *lexer)
{
    lexer_skip_whitespace(lexer);
    lexer_skip_comment(lexer);

    struct token token;
    int comment = last_char(lexer) != '#';
    if (last_char(lexer) == EOF)
    {
        token.type = TOKEN_EOF;
        token.word = NULL;
    }
    else if ((comment && lexer_is_word_char(lexer)) || lexer->escape_next)
    {
        token = lexer_next_handle_word(lexer);
        if (lexer->context == LEXING_COMMAND)
            lex_keyword_or_word(lexer, &token);
    }
    else
        token = lexer_switch(lexer);

    if (TOKEN_IS_CLOSER(token))
        lexer->context = LEXING_COMMAND;
    else
        lexer->context = LEXING_ARGUMENT;

    return token;
}
