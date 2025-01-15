#include "parser.h"

#include <string.h>
/*
rule_for =
    'for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] ) {'\n'} 'do' compound_list 'done' ;
*/
static struct ast *rule_for(struct parser *parser)
{
    struct token tok;
    EXPECT_WORD(parser->lexer, "for", "Expected 'for' (rule_if)\n");

    struct ast *root = ast_new(FOR_LOOP);
    tok = lexer_pop(parser->lexer);
    if (tok.type != TOKEN_WORD)
    {
        parser->status = PARSER_UNEXPECTED_TOKEN;
        fprintf(stderr, "Error: Expected a word token (rule_for)\n");
        return NULL;
    }
    root->left = ast_new(WORD_COMPONENTS);
    root->left->values = malloc(sizeof(struct word *) * 2);
    root->left->values[0] = tok.word;
    root->left->values[1] = NULL;
    root->left->size = 1;

    if (!eat_word(parser->lexer, ";"))
    {
        skip_eol(parser->lexer);
        EXPECT_WORD(parser->lexer, "in", "Expected 'in' (rule_for)\n");
        CHECK_STATUS(parser, root, "Error after parsing word (rule_for)\n");
        struct ast *middle = ast_new(WORD_COMPONENTS);
        middle->values = malloc(sizeof(struct word *) * 16);
        middle->size = 0;
        while (lexer_peek(parser->lexer).type == TOKEN_WORD)
        {
            tok = lexer_pop(parser->lexer);
            if (middle->size >= 16)
            {
                middle->values = realloc(middle->values, sizeof(struct word *) * middle->size * 2);
            }
            middle->values[middle->size++] = tok.word;
            middle->values[middle->size] = NULL;
        }
        root->middle = middle;
        if (!eat(parser->lexer, TOKEN_SEMICOLON) && !eat(parser->lexer, TOKEN_EOL))
        {
            parser->status = PARSER_UNEXPECTED_TOKEN;
            fprintf(stderr, "Error: Expected ';' or EOL (rule_for)\n");
            return NULL;
        }
    }
    skip_eol(parser->lexer);
    EXPECT_WORD(parser->lexer, "do", "Expected 'do' (rule_for)\n");
    CHECK_STATUS(parser, root, "Error after parsing word (rule_for)\n");
    root->right = compound_list(parser);
    CHECK_STATUS(parser, root, "Error after compound_list (rule_for)\n");
    EXPECT_WORD(parser->lexer, "done", "Expected 'done' (rule_for)\n");
    return root;
}

// rule_while = 'while' compound_list 'do' compound_list 'done' ;
static struct ast *rule_while(struct parser *parser)
{
    (void)parser;
    return NULL;
}

// rule_until = 'until' compound_list 'do' compound_list 'done' ;
static struct ast *rule_until(struct parser *parser)
{
    (void)parser;
    return NULL;
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
    struct token tok;
    EXPECT_WORD(parser->lexer, "if", "Expected 'if' keyword (rule_if)\n");
    CHECK_STATUS(parser, NULL, "Error bad if\n");

    struct ast *root = ast_new(CONDITIONS);

    root->left = compound_list(parser);
    IS_BAD_IF(parser);
    CHECK_STATUS(parser, root, "Error after parsing compound_list (if)\n");

    EXPECT_WORD(parser->lexer, "then", "Expected 'then' keyword (rule_if)\n");
    CHECK_STATUS(parser, root, "Error bad if\n");

    root->middle = compound_list(parser);
    IS_BAD_IF(parser);
    CHECK_STATUS(parser, root, "Error after parsing compound_list (if)\n");

    root->right = else_clause(parser);
    IS_BAD_IF(parser);
    CHECK_STATUS(parser, root, "Error after parsing else_clause (if)\n");

    EXPECT_WORD(parser->lexer, "fi", "Expected 'fi' keyword (rule_if)\n");
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
    const char *keyword = lexer_peek(parser->lexer).word->value.data;
    if (strcmp(keyword, "if") == 0)
        return rule_if(parser);
    if (strcmp(keyword, "for") == 0)
        return rule_for(parser);
    if (strcmp(keyword, "while") == 0)
        return rule_while(parser);
    if (strcmp(keyword, "until") == 0)
        return rule_until(parser);
    parser->status = PARSER_UNEXPECTED_TOKEN;
    fprintf(stderr, "Error: Expected a shell command (shell_command)\n");
    return NULL;
}

#include "parser_shell_command.h"
