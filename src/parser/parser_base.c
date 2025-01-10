#include <stdlib.h>
#include <string.h>

#include "parser.h"

#define CHECK_STATUS(parser, ast, msg)                                         \
    if (parser->status != PARSER_OK)                                           \
    {                                                                          \
        if (ast)                                                               \
            free(ast);                                                         \
        fprintf(stderr, msg);                                                  \
        return NULL;                                                           \
    }

#define EXPECT_WORD(lexer, word, msg)                                          \
    if (!eat_word(lexer, word))                                                \
    {                                                                          \
        fprintf(stderr, msg);                                                  \
        parser->status = PARSER_UNEXPECTED_TOKEN;                              \
        exit(2);                                                               \
    }

static int is_closing_word(char *word)
{
    const char *keywords[] = { "then", "fi", "else", "elif" };
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(keywords[i], word) == 0)
            return 1;
    }
    return 0;
}

static int eat(struct lexer *lexer, enum token_type type)
{
    struct token token = lexer_peek(lexer);
    if (token.type == type)
    {
        lexer_pop(lexer);
        return 1;
    }
    return 0;
}

// Eat a word token if it matches the given word and free it's value string
static int eat_word(struct lexer *lexer, const char *word)
{
    struct token token = lexer_peek(lexer);
    if (token.type == TOKEN_WORD && strcmp(token.value, word) == 0)
    {
        free(token.value);
        lexer_pop(lexer);
        return 1;
    }
    return 0;
}

static void skip_eol(struct lexer *lexer)
{
    while (eat(lexer, TOKEN_EOL))
        continue;
}

// {'\n'} and_or { ( ';' | '&' | '\n' ) {'\n'} and_or } [ ';' | '&' ] {'\n'} ;
// no '&' in step 1
struct ast *compound_list(struct parser *parser)
{
    skip_eol(parser->lexer);
    struct ast *root = ast_new(COMMAND_LIST);
    root->left = and_or(parser);
    CHECK_STATUS(parser, root,
                 "Error after parsing and_or (compound_list)[0]\n");

    struct token tok = lexer_peek(parser->lexer);
    struct ast *current = root;
    while (tok.type == TOKEN_SEMICOLON || tok.type == TOKEN_EOL)
    {
        lexer_pop(parser->lexer);
        skip_eol(parser->lexer);

        tok = lexer_peek(parser->lexer);
        // if the next token is a closing keyword, we stop the compound_list
        if (is_closing_word(tok.value))
            break;

        current->right = ast_new(COMMAND_LIST);
        current = current->right;
        current->left = and_or(parser);
        CHECK_STATUS(parser, root,
                     "Error after parsing and_or (compound_list)[1]\n");

        tok = lexer_peek(parser->lexer);
    }

    if (current == root)
    {
        root = root->left;
        free(current);
    }

    return root;
}

/*
else_clause =
    'else' compound_list
    | 'elif' compound_list 'then' compound_list [ else_clause ]
    ;
*/
static struct ast *else_clause(struct parser *parser)
{
    if (eat_word(parser->lexer, "else"))
    {
        struct ast *ast = compound_list(parser);
        CHECK_STATUS(parser, ast, "Error after parsing compound_list (else)\n");
        return ast;
    }
    if (eat_word(parser->lexer, "elif"))
    {
        struct ast *root = ast_new(CONDITIONS);
        root->left = compound_list(parser);
        CHECK_STATUS(parser, root,
                     "Error after parsing compound_list (elif)[0]\n");
        eat_word(parser->lexer, "then");
        root->middle = compound_list(parser);
        CHECK_STATUS(parser, root,
                     "Error after parsing compound_list (elif)[1]\n");
        root->right = else_clause(parser);
        return root;
    }
    return NULL;
}

// 'if' compound_list 'then' compound_list [ else_clause ] 'fi' ;
static struct ast *rule_if(struct parser *parser)
{
    EXPECT_WORD(parser->lexer, "if", "Expected 'if' keyword (rule_if)\n");

    struct ast *root = ast_new(CONDITIONS);

    root->left = compound_list(parser);
    CHECK_STATUS(parser, root, "Error after parsing compound_list (if)\n");

    EXPECT_WORD(parser->lexer, "then", "Expected 'then' keyword (rule_if)\n");

    root->middle = compound_list(parser);
    CHECK_STATUS(parser, root, "Error after parsing compound_list (if)\n");

    root->right = else_clause(parser);
    CHECK_STATUS(parser, root, "Error after parsing else_clause (if)\n");

    EXPECT_WORD(parser->lexer, "fi", "Expected 'fi' keyword (rule_if)\n");

    return root;
}

/*
shell_command =
    '{' compound_list '}'
  | '(' compound_list ')'
  | rule_for
  | rule_while
  | rule_until
  | rule_case
  | rule_if
  ;
*/
struct ast *shell_command(struct parser *parser)
{
    if (strcmp(lexer_peek(parser->lexer).value, "if") == 0)
        return rule_if(parser);
    parser->status = PARSER_UNEXPECTED_TOKEN;
    fprintf(stderr, "Error: Expected a shell command (shell_command)\n");
    return NULL;
}

/*
command =
    simple_command
    | shell_command  { redirection }
    | funcdec { redirection }
    ;
*/
struct ast *command(struct parser *parser)
{
    const char shell_commands[][32] = {
        "if" // only that in step 1
    };
    for (size_t i = 0; i < sizeof(shell_commands) / sizeof(shell_commands[0]);
         i++)
    {
        if (strcmp(shell_commands[i], lexer_peek(parser->lexer).value) == 0)
            return shell_command(parser);
    }
    return simple_command(parser);
}

/*
pipeline = ['!'] command { '|' {'\n'} command } ;
not in step 1
*/
struct ast *pipeline(struct parser *parser)
{
    return command(parser);
}

/*
and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
not in step 1
*/
struct ast *and_or(struct parser *parser)
{
    return pipeline(parser);
}

/*
list = and_or { ( ';' | '&' ) and_or } [ ';' | '&' ] ;
For step 1, only works for ';' and not for '&' yet.
*/
struct ast *list(struct parser *parser)
{
    struct ast *root = ast_new(COMMAND_LIST);
    root->left = and_or(parser);
    CHECK_STATUS(parser, root, "Error after parsing and_or (list)\n");

    struct token token = lexer_peek(parser->lexer);
    struct ast *current = root;
    while (token.type == TOKEN_SEMICOLON) // || AMPERSAND later
    {
        lexer_pop(parser->lexer);
        enum token_type next = lexer_peek(parser->lexer).type;

        if (next == TOKEN_EOF || next == TOKEN_EOL)
            // corresponding to the [ ';' | '&' ] part of the rule
            break;

        current->right = ast_new(COMMAND_LIST);
        current = current->right;
        current->left = and_or(parser);
        CHECK_STATUS(parser, root, "Error after parsing and_or (list)\n");

        token = lexer_peek(parser->lexer);
    }

    if (current == root)
    {
        // if it is a single command, we don't need a COMMAND_LIST node
        root = root->left;
        free(current);
    }

    return root;
}

/*
input =
    list '\n'
    | list EOF
    | '\n'
    | EOF
    ;
*/
struct ast *input(struct parser *parser)
{
    struct token tok = lexer_peek(parser->lexer);
    parser->status = tok.type == TOKEN_EOF ? PARSER_EOF : PARSER_OK;

    if (tok.type == TOKEN_EOL || tok.type == TOKEN_EOF)
        return NULL; // empty command

    struct ast *ast = list(parser);
    CHECK_STATUS(parser, ast, "Error after parsing list (input)\n");

    if (!eat(parser->lexer, TOKEN_EOL) && !eat(parser->lexer, TOKEN_EOF))
        parser->status = PARSER_UNEXPECTED_TOKEN;

    // CHECK_STATUS(parser, ast, "Error after parsing EOL or EOF (input)\n");
    return ast;
}
