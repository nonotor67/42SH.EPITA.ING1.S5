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

// Handle command blocks
struct ast *command_block(struct parser *parser);

// Handle subshells
struct ast *subshell(struct parser *parser);

/*
command =
    simple_command
    | shell_command  { redirection }
    | funcdec { redirection }
    ;
*/
struct ast *command(struct parser *parser);

#endif /* !PARSER_COMMAND_H */
