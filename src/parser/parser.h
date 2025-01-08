#ifndef PARSER_H
#define PARSER_H

#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser_base.h"
#include "parser_redir.h"
#include "parser_struct.h"

// Parse the tokens into an AST
struct ast *parse(enum parser_status *status, struct lexer *lexer);

#endif // !PARSER_H
