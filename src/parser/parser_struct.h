#ifndef PARSER_STRUCT_H
#define PARSER_STRUCT_H

#include "ast/ast.h"
#include "lexer/lexer.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
    PARSER_BAD_IF,
    PARSER_EOF,
};

struct parser
{
    enum parser_status status;
    struct lexer *lexer;
};

// Used in the rule_if function
int is_closing_word(struct word *word);

// Eat a token if it matches the given type
int eat(struct lexer *lexer, enum token_type type);

// Eat a word token if it matches the given word and free it's value string
int eat_keyword(struct lexer *lexer, char *word);

// Skip all EOL tokens
void skip_eol(struct lexer *lexer);

// Macros to check the parser status
// Transform the parser status to PARSER_BAD_IF if it is not PARSER_OK
#define IS_BAD_IF(parser)                                                      \
    if (parser->status != PARSER_OK)                                           \
    {                                                                          \
        parser->status = PARSER_BAD_IF;                                        \
    }

// Check the parser status and free the AST if it is not PARSER_OK
// Empty the lexer if the status is not PARSER_OK
#define CHECK_STATUS(parser, ast, msg)                                         \
    if (parser->status != PARSER_OK)                                           \
    {                                                                          \
        if (ast)                                                               \
            ast_free(ast);                                                     \
        fprintf(stderr, msg);                                                  \
        tok = lexer_pop(parser->lexer);                                        \
        while (tok.type != TOKEN_EOF)                                          \
        {                                                                      \
            if (tok.word)                                                      \
                word_free(tok.word);                                           \
            tok = lexer_pop(parser->lexer);                                    \
        }                                                                      \
        return NULL;                                                           \
    }

// Expect a word token and free it's value string if it is not the expected word
#define EXPECT_KEYWORD(lexer, expected, msg)                                   \
    if (!eat_keyword(lexer, expected))                                         \
    {                                                                          \
        fprintf(stderr, msg);                                                  \
        parser->status = PARSER_UNEXPECTED_TOKEN;                              \
        struct token token = lexer_peek(lexer);                                \
        if (token.word)                                                        \
            word_free(token.word);                                             \
        lexer_pop(lexer);                                                      \
    }

// Check if a pointer is NULL and exit if it is
#define CHECK_ALLOCATION(ptr)                                                  \
    if (!ptr)                                                                  \
    {                                                                          \
        fprintf(stderr, "Failed allocating memory\n");                         \
        exit(1);                                                               \
    }

// Expect a token type and set the parser status to PARSER_UNEXPECTED_TOKEN
// if it is not the expected type
#define EXPECT_TOKEN(lexer, type, msg)                                         \
    if (!eat(lexer, type))                                                     \
    {                                                                          \
        fprintf(stderr, msg);                                                  \
        parser->status = PARSER_UNEXPECTED_TOKEN;                              \
        return NULL;                                                           \
    }

#endif /* !PARSER_STRUCT_H */
