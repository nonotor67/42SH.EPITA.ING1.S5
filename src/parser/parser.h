#ifndef PARSER_H
#define PARSER_H

#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser_command.h"
#include "parser_operator.h"
#include "parser_shell_command.h"
#include "parser_struct.h"

struct parser *parser_new(struct lexer *lexer);
void parser_free(struct parser *parser);

// Parse the tokens into an AST
struct ast *parse(struct parser *parser);

/*
list = and_or { ( ';' | '&' ) and_or } [ ';' | '&' ] ;
For step 1, only works for ';' and not for '&' yet.
*/
struct ast *list(struct parser *parser);

/*
input =
    list '\n'
    | list EOF
    | '\n'
    | EOF
    ;
*/
struct ast *input(struct parser *parser);

#endif /* !PARSER_H */
