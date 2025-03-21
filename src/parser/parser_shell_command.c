#include "parser_shell_command.h"

#include <string.h>

static void set_error(struct parser *parser, char *msg)
{
    parser->status = PARSER_UNEXPECTED_TOKEN;
    fprintf(stderr, "Error: %s\n", msg);
}

static struct ast *create_word(struct token tok)
{
    struct ast *root = ast_new(WORD_COMPONENTS);
    root->values = malloc(sizeof(struct word *) * 2);
    root->values[0] = tok.word;
    root->values[1] = NULL;
    root->size = 1;
    return root;
}

// Loop inside in
static void rule_for_handle_in(struct parser *parser, struct ast *middle)
{
    struct token tok;
    while (lexer_peek(parser->lexer).type == TOKEN_WORD)
    {
        tok = lexer_pop(parser->lexer);
        if (middle->size >= 16)
        {
            middle->values = realloc(middle->values,
                                     sizeof(struct word *) * middle->size * 2);
        }
        middle->values[middle->size++] = tok.word;
        middle->values[middle->size] = NULL;
    }
}

/*
rule_for =
    'for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] ) {'\n'} 'do'
compound_list 'done' ;
*/
// TODO: Refactor this function if adding more rules (40 lines)
static struct ast *rule_for(struct parser *parser)
{
    struct token tok;
    EXPECT_KEYWORD(parser->lexer, "for", "Expected 'for' (rule_if)\n");

    struct ast *root = ast_new(FOR_LOOP);
    tok = lexer_pop(parser->lexer);
    if (tok.type != TOKEN_WORD)
    {
        set_error(parser, "Expected a word token (rule_for)\n");
        return NULL;
    }
    root->left = create_word(tok);

    lexer_context_begin(parser->lexer);
    if (!eat(parser->lexer, TOKEN_SEMICOLON))
    {
        skip_eol(parser->lexer);
        tok = lexer_peek(parser->lexer);
        if (tok.type == TOKEN_KEYWORD
            && strcmp(tok.word->value.data, "in") == 0)
        {
            EXPECT_KEYWORD(parser->lexer, "in", "Expected 'in' (rule_for)\n");
            CHECK_STATUS(parser, root, "Error after parsing word (rule_for)\n");
            struct ast *middle = ast_new(WORD_COMPONENTS);
            middle->values = malloc(sizeof(struct word *) * 16);
            middle->size = 0;
            rule_for_handle_in(parser, middle);
            root->middle = middle;
            middle->values[middle->size] = NULL;

            if (!eat(parser->lexer, TOKEN_SEMICOLON)
                && !eat(parser->lexer, TOKEN_EOL))
            {
                set_error(parser, "Expected ';' or EOL (rule_for)\n");
                return NULL;
            }
        }
    }
    skip_eol(parser->lexer);
    EXPECT_KEYWORD(parser->lexer, "do", "Expected 'do' (rule_for)\n");
    CHECK_STATUS(parser, root, "Error after parsing word (rule_for)\n");
    lexer_context_begin(parser->lexer);
    root->right = compound_list(parser);
    CHECK_STATUS(parser, root, "Error after compound_list (rule_for)\n");
    EXPECT_KEYWORD(parser->lexer, "done", "Expected 'done' (rule_for)\n");
    return root;
}

// rule_while = 'while' compound_list 'do' compound_list 'done' ;
static struct ast *rule_while(struct parser *parser)
{
    struct token tok;
    EXPECT_KEYWORD(parser->lexer, "while",
                   "Expected 'while' keyword (rule_while)\n");
    CHECK_STATUS(parser, NULL, "Error bad while\n");

    struct ast *root = ast_new(WHILE_LOOP);

    root->left = compound_list(parser);
    CHECK_STATUS(parser, root,
                 "Error after parsing compound_list (while[0])\n");

    EXPECT_KEYWORD(parser->lexer, "do", "Expected 'do' keyword (rule_while)\n");
    CHECK_STATUS(parser, root, "Error bad while\n");
    lexer_context_begin(parser->lexer);
    root->right = compound_list(parser);
    CHECK_STATUS(parser, root,
                 "Error after parsing compound_list (while[1])\n");

    EXPECT_KEYWORD(parser->lexer, "done",
                   "Expected 'done' keyword (rule_while)\n");
    CHECK_STATUS(parser, root, "Error bad while\n");

    return root;
}

// rule_until = 'until' compound_list 'do' compound_list 'done' ;
static struct ast *rule_until(struct parser *parser)
{
    struct token tok;
    EXPECT_KEYWORD(parser->lexer, "until",
                   "Expected 'until' keyword (rule_until)\n");
    CHECK_STATUS(parser, NULL, "Error bad until\n");

    struct ast *root = ast_new(UNTIL_LOOP);

    root->left = compound_list(parser);
    CHECK_STATUS(parser, root,
                 "Error after parsing compound_list (until[0])\n");

    EXPECT_KEYWORD(parser->lexer, "do", "Expected 'do' keyword (rule_until)\n");
    CHECK_STATUS(parser, root, "Error bad until\n");
    lexer_context_begin(parser->lexer);

    root->right = compound_list(parser);
    CHECK_STATUS(parser, root,
                 "Error after parsing compound_list (until[1])\n");

    EXPECT_KEYWORD(parser->lexer, "done",
                   "Expected 'done' keyword (rule_until)\n");
    CHECK_STATUS(parser, root, "Error bad until\n");

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
    struct token tok;
    if (eat_keyword(parser->lexer, "else"))
    {
        struct ast *ast = compound_list(parser);
        CHECK_STATUS(parser, ast, "Error after parsing compound_list (else)\n");
        return ast;
    }
    if (eat_keyword(parser->lexer, "elif"))
    {
        struct ast *root = ast_new(CONDITIONS);
        root->left = compound_list(parser);
        CHECK_STATUS(parser, root,
                     "Error after parsing compound_list (elif)[0]\n");
        eat_keyword(parser->lexer, "then");
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
    struct token tok;
    EXPECT_KEYWORD(parser->lexer, "if", "Expected 'if' keyword (rule_if)\n");
    CHECK_STATUS(parser, NULL, "Error bad if\n");

    struct ast *root = ast_new(CONDITIONS);

    lexer_context_begin(parser->lexer);
    root->left = compound_list(parser);
    IS_BAD_IF(parser);
    CHECK_STATUS(parser, root, "Error after parsing compound_list (if)\n");

    EXPECT_KEYWORD(parser->lexer, "then",
                   "Expected 'then' keyword (rule_if)\n");
    CHECK_STATUS(parser, root, "Error bad if\n");
    // begin new command after then
    lexer_context_begin(parser->lexer);

    root->middle = compound_list(parser);
    IS_BAD_IF(parser);
    CHECK_STATUS(parser, root, "Error after parsing compound_list (if)\n");

    root->right = else_clause(parser);
    IS_BAD_IF(parser);
    CHECK_STATUS(parser, root, "Error after parsing else_clause (if)\n");

    EXPECT_KEYWORD(parser->lexer, "fi", "Expected 'fi' keyword (rule_if)\n");
    CHECK_STATUS(parser, root, "Error bad if\n");

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
    struct token tok;
    tok = lexer_peek(parser->lexer);
    if (tok.type == TOKEN_LEFT_PAREN)
        return subshell(parser);
    if (tok.type == TOKEN_KEYWORD || tok.type == TOKEN_WORD)
    {
        const char *keyword = tok.word->value.data;
        if (strcmp(keyword, "if") == 0)
            return rule_if(parser);
        if (strcmp(keyword, "for") == 0)
            return rule_for(parser);
        if (strcmp(keyword, "while") == 0)
            return rule_while(parser);
        if (strcmp(keyword, "until") == 0)
            return rule_until(parser);
        if (strcmp(keyword, "{") == 0)
            return command_block(parser);
    }
    parser->status = PARSER_UNEXPECTED_TOKEN;
    fprintf(stderr, "Error: Expected a shell command (shell_command)\n");
    return NULL;
}
