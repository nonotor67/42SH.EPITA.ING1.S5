#ifndef PARSER_H
#define PARSER_H

#include "ast/ast.h"
#include "lexer/lexer.h"

#include "parser_base.h"
#include "parser_struct.h"
#include "parser_redir.h"

struct parser *parser_new(struct lexer *lexer);
void parser_free(struct parser *parser);

// Parse the tokens into an AST
struct ast *parse(struct parser *parser);

#endif // !PARSER_H
