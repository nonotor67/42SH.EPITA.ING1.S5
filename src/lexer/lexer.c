#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

/*
#####################################
Utilis
#####################################
*/

// Skip the whitespace characters in the data
static void lexer_skip_whitespace(struct lexer *lexer)
{
    while (isspace(lexer->data[lexer->pos]))
    {
        lexer->pos++;
    }
}

// Test if a character is a word character
static int lexer_is_alphanum(struct lexer *lexer)
{
    return isalnum(lexer->data[lexer->pos]);
}

// Handle the words corresponding to keywords
static struct token lexer_next_handle_key(struct token token)
{
    if (strcmp(token.value, "if") == 0)
        token.type = TOKEN_IF;
    else if (strcmp(token.value, "then") == 0)
        token.type = TOKEN_THEN;
    else if (strcmp(token.value, "else") == 0)
        token.type = TOKEN_ELSE;
    else if (strcmp(token.value, "elif") == 0)
        token.type = TOKEN_ELIF;
    else if (strcmp(token.value, "fi") == 0)
        token.type = TOKEN_FI;

    return token;
}

// Create a word token or a keyword token
static struct token lexer_next_handle_word(struct lexer *lexer)
{
    size_t start = lexer->pos;
    while (lexer_is_alphanum(lexer))
        lexer->pos++;

    size_t len = lexer->pos - start;
    char *value = malloc(len + 1);
    if (!value)
    {
        fprintf(stderr, "Failed allocating memory for token value\n");
        return (struct token){ TOKEN_EOF, NULL };
    }

    strncpy(value, lexer->data + start, len);
    value[len] = '\0';

    struct token token;
    token.type = TOKEN_WORD;
    token.value = value;

    return lexer_next_handle_key(token);
}

// Switch the token based on the character
static struct token lexer_switch(struct lexer *lexer)
{
    struct token token;
    token.value = NULL;
    switch (lexer->data[lexer->pos])
    {
    case '\n':
        token.type = TOKEN_EOL;
        lexer->pos++;
        break;
    case ';':
        token.type = TOKEN_SEMICOLON;
        lexer->pos++;
        break;
    case '\'':
        token.type = TOKEN_QUOTE;
        lexer->pos++;
        break;
    case '"':
        token.type = TOKEN_DOUBLE_QUOTE;
        lexer->pos++;
        break;
    case '#':
        token.type = TOKEN_COMMENT;
        lexer->pos++;
        break;
    default:
        token.type = TOKEN_EOF;
        break;
    }

    return token;
}

// Exports

struct lexer *lexer_new(const char *data)
{
    struct lexer *lexer = malloc(sizeof(struct lexer));
    if (!lexer)
    {
        fprintf(stderr, "Failed allocating memory for lexer\n");
        return NULL;
    }
    lexer->data = data;
    lexer->pos = 0;
    lexer->current.type = TOKEN_EOL;
    lexer->current.value = NULL;

    return lexer;
}

void lexer_free(struct lexer *lexer)
{
    free(lexer);
}

struct token lexer_next(struct lexer *lexer)
{
    lexer_skip_whitespace(lexer);

    if (lexer->data[lexer->pos] == '\0')
    {
        struct token token;
        token.type = TOKEN_EOF;
        token.value = NULL;
        return token;
    }

    if (lexer_is_alphanum(lexer))
        return lexer_next_handle_word(lexer);

    return lexer_switch(lexer);
}

struct token lexer_peek(struct lexer *lexer)
{
    size_t pos = lexer->pos;
    struct token token = lexer_next(lexer);
    lexer->pos = pos;
    return token;
}

struct token lexer_pop(struct lexer *lexer)
{
    struct token token = lexer_next(lexer);
    lexer->current = token;
    return token;
}

void lexer_refill(struct lexer *lexer)
{
    if (lexer == NULL)
    {
        fprintf(stderr, "Lexer is NULL\n");
        return;
    }
    // TODO
    // Call the read function to refill the data field
}

void lexer_print(struct lexer *lexer)
{
    size_t pos = lexer->pos;
    printf("Current position : %ld\n\n", pos);
    printf("Current token :\n");
    token_print(lexer->current);
    puts("\n");
    printf("Data :\n%s\n", lexer->data);
    for (size_t i = 0; i < pos; i++)
        putchar(' ');
    putchar('^');
    putchar('\n');
    puts("---------------------");
    printf("Tokens :\n");
    struct token token = lexer_pop(lexer);
    token_print(token);
    putchar('\n');
    while (token.type != TOKEN_EOF)
    {
        token = lexer_pop(lexer);
        token_print(token);
        putchar('\n');
    }
    lexer->pos = pos;
}
