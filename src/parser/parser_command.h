#ifndef PARSER_COMMAND_H
#define PARSER_COMMAND_H

#include "ast/ast.h"
#include "lexer/lexer.h"
#include "parser_struct.h"

// simple_command = WORD { element }
// element = WORD
// Step 1
struct ast *simple_command(struct parser *parser);

// {'\n'} and_or { ( ';' | '&' | '\n' ) {'\n'} and_or } [ ';' | '&' ] {'\n'} ;
// no '&' in step 1
struct ast *compound_list(struct parser *parser);

/*
shell_command =
    '{' compound_list '}'
  | '(' compound_list ')'
  | rule_for
  | rule_while
  | rule_until
  | rule_case
  | rule_if
  ;
*/
struct ast *shell_command(struct parser *parser);

/*
command =
    simple_command
    | shell_command  { redirection }
    | funcdec { redirection }
    ;
*/
struct ast *command(struct parser *parser);

#endif /* !PARSER_COMMAND_H */
