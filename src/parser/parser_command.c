#include <stdlib.h>
#include <string.h>

#include "parser.h"

/*
Contains all the functions to parse the command part of the grammar
--> Simple commands
--> Compound lists
--> If statements
--> Shell commands
--> Commands
*/

/*
redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' ) WORD
--> First : TOKEN_REDIR
*/
static void handle_redirection(struct parser *parser, struct ast *node,
                               int buffer_size_redir)
{
    if (lexer_peek(parser->lexer).type == TOKEN_REDIR)
    {
        if (!node->redir)
        {
            node->redir = malloc(sizeof(struct word *) * buffer_size_redir);
            CHECK_ALLOCATION(node->redir);
        }
        if (node->redir_size >= buffer_size_redir - 1)
        {
            buffer_size_redir *= 2;
            node->redir =
                realloc(node->redir, sizeof(struct word *) * buffer_size_redir);
            CHECK_ALLOCATION(node->redir);
        }
        struct word *redir = lexer_pop(parser->lexer).word;
        node->redir[node->redir_size++] = redir;
        struct token token = lexer_peek(parser->lexer);
        if (token.type == TOKEN_WORD)
        {
            node->redir[node->redir_size++] = token.word;
            lexer_pop(parser->lexer);
        }
        else
        {
            fprintf(stderr, "Error: Expected a word token after redirection\n");
            parser->status = PARSER_UNEXPECTED_TOKEN;
        }
    }
}

/*
prefix = redirection
        | ASSIGNMENT_WORD
First : TOKEN_REDIR, TOKEN_WORD
*/

/* simple_command = prefix { prefix }
                  | { prefix } WORD { element }
First : TOKEN_REDIR, TOKEN_WORD

element = WORD | redirection
Step 2
 */
struct ast *simple_command(struct parser *parser)
{
    struct token token = lexer_peek(parser->lexer);
    if (token.type == TOKEN_WORD || token.type == TOKEN_REDIR)
    {
        struct ast *node = ast_new(SIMPLE_COMMAND);
        int buffer_size_values = 16;
        int buffer_size_redir = 16;
        node->size = 0;
        node->redir_size = 0;
        node->values = malloc(sizeof(struct word *) * buffer_size_values);
        CHECK_ALLOCATION(node->values);
        while (token.type == TOKEN_WORD || token.type == TOKEN_REDIR)
        {
            if (token.type != TOKEN_REDIR)
                lexer_pop(parser->lexer);
            if (node->size >= buffer_size_values - 1)
            {
                buffer_size_values *= 2;
                node->values =
                    realloc(node->values, sizeof(char *) * buffer_size_values);
                CHECK_ALLOCATION(node->values);
            }
            handle_redirection(parser, node, buffer_size_redir);
            struct token tok;
            CHECK_STATUS(parser, node,
                         "Error after parsing redirection (simple_command)\n");
            if (token.type == TOKEN_WORD)
            {
                node->values[node->size] = token.word;
                node->size++;
            }
            token = lexer_peek(parser->lexer);
        }
        node->values[node->size] = NULL;
        return node;
    }
    fprintf(stderr, "Error: Expected a word token (simple_command)\n");
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return NULL;
}

// {'\n'} and_or { ( ';' | '&' | '\n' ) {'\n'} and_or } [ ';' | '&' ] {'\n'} ;
// no '&' in step 1
struct ast *compound_list(struct parser *parser)
{
    skip_eol(parser->lexer);
    struct ast *root = ast_new(COMMAND_LIST);
    root->left = and_or(parser);
    struct token tok;
    CHECK_STATUS(parser, root,
                 "Error after parsing and_or (compound_list)[0]\n");

    tok = lexer_peek(parser->lexer);
    struct ast *current = root;
    while (tok.type == TOKEN_SEMICOLON || tok.type == TOKEN_EOL)
    {
        lexer_pop(parser->lexer);
        skip_eol(parser->lexer);

        tok = lexer_peek(parser->lexer);
        // if the next token is a closing keyword, we stop the compound_list
        if (tok.type == TOKEN_WORD && is_closing_word(tok.word))
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
    if (strcmp(lexer_peek(parser->lexer).word->value.data, "if") == 0)
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
        struct token token = lexer_peek(parser->lexer);
        if (token.type == TOKEN_WORD)
            if (strcmp(shell_commands[i], token.word->value.data) == 0)
            {
                struct token tok;
                struct ast *ast = shell_command(parser);
                CHECK_STATUS(parser, ast,
                             "Error after parsing shell_command (command)\n");
                int buffer_size_redir = 16;
                while (lexer_peek(parser->lexer).type == TOKEN_REDIR)
                {
                    handle_redirection(parser, ast, buffer_size_redir);
                }
                return ast;
            }
    }
    return simple_command(parser);
}
