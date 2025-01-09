#ifndef PARSER_BASE_H
#define PARSER_BASE_H

#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser_struct.h"

struct ast *compound_list(struct parser *parser);

struct ast *shell_command(struct parser *parser);

struct ast *simple_command(struct parser *parser);

struct ast *command(struct parser *parser);

struct ast *pipeline(struct parser *parser);

struct ast *and_or(struct parser *parser);

struct ast *list(struct parser *parser);

struct ast *input(struct parser *parser);

#endif /* !PARSER_BASE_H */
