#include <ast/ast.h>
#include <execution/execution.h>
#include <io/io.h>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <stddef.h>
#include <utils/utils.h>

static struct HashMap *init_hashmap(void)
{
    // Create a new hash table
    struct HashMap *ht = create_hash_table();
    if (!ht)
    {
        fprintf(stderr, "42sh: failed to create the hash table\n");
        exit(1);
    }

    // Insert the default variables
    insertVariable(ht, "PWD", getenv("PWD"));

    return ht;
}

int main(int argc, char **argv)
{
    struct HashMap *ht = init_hashmap();
    if (!ht)
    {
        fprintf(stderr, "42sh: failed to initialize the hash table\n");
        exit(1);
    }
    // Create a reader, lexer, and parser.
    struct reader *reader = reader_new(argc, argv);
    struct lexer *lexer = lexer_new(reader);
    // Default
    int res = 0;
    while (lexer->current.type != TOKEN_EOF)
    {
        struct parser *parser = parser_new(lexer);

        // Parse the input.
        struct ast *ast = parse(parser);

        // Execute the AST.
        if (ast)
            res = execution(ast);
        if (parser->status == PARSER_BAD_IF)
            res = 2;
        parser_free(parser);
        ast_free(ast);
    }

    reader_free(reader);
    lexer_free(lexer);

    free_hash_map(ht);

    return res;
}
