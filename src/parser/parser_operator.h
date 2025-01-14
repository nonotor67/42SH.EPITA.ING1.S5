#ifndef PARSER_OPERATOR_H
#define PARSER_OPERATOR_H

#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser_struct.h"

/*
pipeline = ['!'] command { '|' {'\n'} command } ;
not in step 1
*/
struct ast *pipeline(struct parser *parser);

/*
and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
not in step 1
*/
struct ast *and_or(struct parser *parser);

#endif /* !PARSER_OPERATOR_H */