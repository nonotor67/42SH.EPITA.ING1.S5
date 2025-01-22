#ifndef WORD_H
#define WORD_H

#include <stddef.h>

#include "utils/strings.h"

// either name or commands is null
struct variable
{
    size_t pos; // position in the string
    struct string name;
    struct ast *commands; // vector of ASTs
    int is_quoted;
};

struct word
{
    struct string value;
    struct variable *variables;
    size_t var_length;
    size_t var_capacity;
    int has_escaped; // If the word has escaped characters
    int valid_assignment; // If the word can be an assignment
};

struct word *word_new(void);
void word_free(struct word *word);

// Create a copy of the word
void word_copy(struct word *word, struct word *copy);

// Push a character to the word
void word_push(struct word *word, char c);

// Push a variable to the word
void word_push_variable(struct word *word, struct variable var);

// returns 1 if the word cannot be expanded, 0 otherwise
int is_word_valid(struct word *word);

// returns a new string with variables evaluated
char **word_eval(struct word *word);

#endif // !WORD_H
