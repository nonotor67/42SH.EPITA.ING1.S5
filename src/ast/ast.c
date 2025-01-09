#include "ast.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
            free(ast->values[i]);
        free(ast->values);
    }

    free(ast);
}

static void ast_print_node(struct ast *ast, int file)
{
    switch (ast->type)
    {
    case SIMPLE_COMMAND:
        dprintf(file, "SIMPLE_COMMAND\\n");
        for (int i = 0; i < ast->size; i++)
            dprintf(file, "%s ", ast->values[i]);
        break;
    case COMMAND_LIST:
        dprintf(file, "COMMAND_LIST\\n");
        for (int i = 0; i < ast->size; i++)
            dprintf(file, "%s ", ast->values[i]);
        break;
    case CONDITIONS:
        dprintf(file, "CONDITIONS\\n");
        for (int i = 0; i < ast->size; i++)
            dprintf(file, "%s ", ast->values[i]);
        break;
    }
}

static void ast_print_help(struct ast *ast, int file)
{
    static int id = 0;
    int my_id = id++;

    dprintf(file, "    %d [label=\"", my_id);

    ast_print_node(ast, file);

    dprintf(file, "\"]\n");

    if (ast->left)
    {
        dprintf(file, "    %d -> %d\n", my_id, id);
        ast_print_help(ast->left, file);
    }

    if (ast->middle)
    {
        dprintf(file, "    %d -> %d\n", my_id, id);
        ast_print_help(ast->middle, file);
    }

    if (ast->right)
    {
        dprintf(file, "    %d -> %d\n", my_id, id);
        ast_print_help(ast->right, file);
    }
}

void ast_print(struct ast *ast)
{
    int file = open("ast.dot", O_CREAT | O_WRONLY, 0644);
    dprintf(file, "digraph G {\n");
    ast_print_help(ast, file);
    dprintf(file, "}\n");
}

void ast_print_dot(struct ast *ast)
{
    ast_print(ast);
    system("dot -Tpng ast.dot -o ast.png");
    system("xdg-open ast.png");
}
