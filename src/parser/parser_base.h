#ifndef PARSER_BASE_H
#define PARSER_BASE_H

#include "parser_struct.h"
#include "ast/ast.h"
#include "lexer/lexer.h"

struct ast *simple_command(enum parser_status *status, struct lexer *lexer);

struct ast *command(enum parser_status *status, struct lexer *lexer);

struct ast *pipeline(enum parser_status *status, struct lexer *lexer);

struct ast *and_or(enum parser_status *status, struct lexer *lexer);

struct ast *list(enum parser_status *status, struct lexer *lexer);

struct ast *input(enum parser_status *status, struct lexer *lexer);

#endif /* !PARSER_BASE_H */
