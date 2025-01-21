#ifndef PARSER_SHELL_COMMAND_H
#define PARSER_SHELL_COMMAND_H

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

#endif //! PARSER_SHELL_COMMAND_H
