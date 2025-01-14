#include "ast.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils/utils.h"

struct ast *ast_new(enum ast_type type)
{
    struct ast *ast = malloc(sizeof(struct ast));
    if (!ast)
    {
        fprintf(stderr, "Failed allocating memory for AST node\n");
        return NULL;
    }

    ast->type = type;
    ast->left = NULL;
    ast->right = NULL;
    ast->middle = NULL;
    ast->size = 0;
    ast->values = NULL;
    ast->expanded_values = NULL;
    ast->expanded_redir = NULL;
    ast->redir = NULL;
    ast->redir_size = 0;

    return ast;
}

void ast_free(struct ast *ast)
{
    if (ast == NULL)
        return;

    if (ast->left)
        ast_free(ast->left);
    if (ast->right)
        ast_free(ast->right);
    if (ast->middle)
        ast_free(ast->middle);

    if (ast->values)
    {
        for (size_t i = 0; ast->values[i]; i++)
            word_free(ast->values[i]);
        free(ast->values);
    }
    if (ast->expanded_values)
    {
        for (size_t i = 0; ast->expanded_values[i]; i++)
            free(ast->expanded_values[i]);
        free(ast->expanded_values);
    }
    if (ast->redir)
    {
        for (size_t i = 0; ast->redir[i]; i++)
            free(ast->redir[i]);
        free(ast->redir);
    }

    free(ast);
}

static void ast_print_node(struct ast *ast, FILE *file)
{
    switch (ast->type)
    {
    case SIMPLE_COMMAND:
        fprintf(file, "SIMPLE_COMMAND\\n");
        break;
    case COMMAND_LIST:
        fprintf(file, "COMMAND_LIST\\n");
        break;
    case CONDITIONS:
        fprintf(file, "CONDITIONS\\n");
        break;
    case WHILE_LOOP:
        fprintf(file, "WHILE_LOOP\\n");
        break;
    case FOR_LOOP:
        fprintf(file, "FOR_LOOP\\n");
        break;
    case PIPELINE:
        fprintf(file, "PIPELINE\\n");
        break;
    case NEGATION:
        fprintf(file, "NEGATION\\n");
        break;
    case UNTIL_LOOP:
        fprintf(file, "UNTIL_LOOP\\n");
        break;
    case AND:
        fprintf(file, "AND\\n");
        break;
    case OR:
        fprintf(file, "OR\\n");
        break;
    default:
        fprintf(file, "UNKNOWN\\n");
        break;
    }
    for (int i = 0; i < ast->size; i++)
        fprintf(file, "%s ", ast->values[i]->value.data);
    for (int i = 0; i < ast->size; i++)
        fprintf(file, "%s ", ast->redir[i]->value.data);
}

static void ast_print_help(struct ast *ast, FILE *file)
{
    static int id = 0;
    int my_id = id++;

    fprintf(file, "    %d [label=\"", my_id);

    ast_print_node(ast, file);

    fprintf(file, "\"]\n");

    if (ast->left)
    {
        fprintf(file, "    %d -> %d\n", my_id, id);
        ast_print_help(ast->left, file);
    }

    if (ast->middle)
    {
        fprintf(file, "    %d -> %d\n", my_id, id);
        ast_print_help(ast->middle, file);
    }

    if (ast->right)
    {
        fprintf(file, "    %d -> %d\n", my_id, id);
        ast_print_help(ast->right, file);
    }
}

void ast_print(struct ast *ast)
{
    FILE *file = fopen("ast.dot", "w+");
    fprintf(file, "digraph G {\n");
    ast_print_help(ast, file);
    fprintf(file, "}\n");
    fclose(file);
}
