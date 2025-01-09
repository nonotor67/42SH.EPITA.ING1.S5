#ifndef PARSER_BASE_H
#define PARSER_BASE_H

#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser_struct.h"

struct ast *simple_command(struct parser *parser, struct lexer *lexer);

struct ast *command(struct parser *parser, struct lexer *lexer);

struct ast *pipeline(struct parser *parser, struct lexer *lexer);

struct ast *and_or(struct parser *parser, struct lexer *lexer);

struct ast *list(struct parser *parser, struct lexer *lexer);

struct ast *input(struct parser *parser, struct lexer *lexer);

#endif /* !PARSER_BASE_H */
