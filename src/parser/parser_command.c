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
            node->redir_size = 0;
            CHECK_ALLOCATION(node->redir);
        }
        if (node->redir_size >= buffer_size_redir - 2)
        {
            buffer_size_redir *= 2;
            node->redir =
                realloc(node->redir, sizeof(struct word *) * buffer_size_redir);
            CHECK_ALLOCATION(node->redir);
        }
        struct word *redir = lexer_pop(parser->lexer).word;
        node->redir[node->redir_size++] = redir;
        node->redir[node->redir_size] = NULL;
        struct token token = lexer_peek(parser->lexer);
        if (token.type == TOKEN_WORD)
        {
            node->redir[node->redir_size++] = token.word;
            node->redir[node->redir_size] = NULL;
            lexer_pop(parser->lexer);
        }
        else
        {
            fprintf(stderr, "Error: Expected a word token after redirection\n");
            parser->status = PARSER_UNEXPECTED_TOKEN;
        }
    }
}

// funcdec = WORD '(' ')' {'\n'} shell_command ;
static struct ast *handle_func(struct parser *parser, struct ast *node,
                               struct token token)
{
    lexer_pop(parser->lexer);
    // cleans the ancien node
    free(node->values);
    free(node);
    struct ast *func = ast_new(FUNCTION);
    func->values = malloc(sizeof(struct word *) * 2);
    CHECK_ALLOCATION(func->values);
    func->size = 0;
    func->values[func->size++] = token.word;
    func->values[func->size] = NULL;
    struct token tok = lexer_pop(parser->lexer);
    if (tok.type == TOKEN_RIGHT_PAREN)
    {
        skip_eol(parser->lexer);
        func->left = shell_command(parser);
        CHECK_STATUS(parser, func,
                     "Error after parsing shell_command (handle_func)\n");
        return func;
    }
    fprintf(stderr, "Error: Expected a right parenthesis in function\n");
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return NULL;
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

The step 3 needs the simple command to handle function
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
        int is_func = 1;
        while (token.type == TOKEN_WORD || token.type == TOKEN_REDIR)
        {
            if (token.type != TOKEN_REDIR)
            {
                lexer_pop(parser->lexer);
                // Handles function creation
                if (is_func
                    && lexer_peek(parser->lexer).type == TOKEN_LEFT_PAREN)
                {
                    return handle_func(parser, node, token);
                }
            }
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
            is_func = 0;
        }
        node->values[node->size] = NULL;
        if (node->redir)
            node->redir[node->redir_size] = NULL;
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
        if ((tok.type == TOKEN_KEYWORD && is_closing_word(tok.word))
            || (tok.type == TOKEN_WORD
                && strcmp(tok.word->value.data, "}") == 0)
            || tok.type == TOKEN_RIGHT_PAREN)
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
command =
    simple_command
    | shell_command  { redirection }
    | funcdec { redirection }
    ;
*/
struct ast *command(struct parser *parser)
{
    struct token token = lexer_peek(parser->lexer);
    struct ast *ast = NULL;
    if (token.type == TOKEN_KEYWORD || token.type == TOKEN_LEFT_PAREN
        || (token.type == TOKEN_WORD
            && strcmp(token.word->value.data, "{") == 0))
    {
        struct token tok;
        ast = shell_command(parser);
        CHECK_STATUS(parser, ast,
                     "Error after parsing shell_command (command)\n");
        int buffer_size_redir = 16;
        while (lexer_peek(parser->lexer).type == TOKEN_REDIR)
            handle_redirection(parser, ast, buffer_size_redir);
        if (ast->redir)
            ast->redir[ast->redir_size] = NULL;
    }
    else
        // Handles functions as the first is the same
        ast = simple_command(parser);
    return ast;
}

// Handle the command block
struct ast *command_block(struct parser *parser)
{
    struct token tok;
    tok = lexer_pop(parser->lexer);
    // Is the opening bracket
    word_free(tok.word);
    lexer_context_begin(parser->lexer);
    struct ast *root = ast_new(COMMAND_BLOCK);
    root->left = compound_list(parser);
    CHECK_STATUS(parser, root,
                 "Error parsing compound list in command block\n");
    tok = lexer_pop(parser->lexer);
    if (tok.type == TOKEN_WORD && strcmp(tok.word->value.data, "}") == 0)
    {
        word_free(tok.word);
        return root;
    }
    fprintf(stderr, "Error: Expected a closing bracket in command block\n");
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return NULL;
}

// Handle shubshells
struct ast *subshell(struct parser *parser)
{
    struct token tok;
    lexer_pop(parser->lexer);
    struct ast *root = ast_new(SUBSHELL);
    root->left = compound_list(parser);
    CHECK_STATUS(parser, root, "Error parsing compound list in Subshell\n");
    tok = lexer_pop(parser->lexer);
    if (tok.type == TOKEN_RIGHT_PAREN)
    {
        return root;
    }
    fprintf(stderr, "Error: Expected a closing parentesis in subshell\n");
    parser->status = PARSER_UNEXPECTED_TOKEN;
    return NULL;
}
