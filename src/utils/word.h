#ifndef WORD_H
#define WORD_H

#include <stddef.h>

#include "utils/strings.h"

struct variable
{
    size_t pos; // position in the string
    struct string name;
};

struct word
{
    struct string value;
    struct variable *variables;
    size_t var_length;
    size_t var_capacity;
};

struct word *word_new(void);
void word_free(struct word *word);

// Push a character to the word
void word_push(struct word *word, char c);

// Push a variable to the word
void word_push_variable(struct word *word, struct variable var);

// returns 1 if words are equal, 0 otherwise
int word_equals(struct word *word1, struct word *word2);

// returns a new string with variables evaluated
char *word_eval(struct word *word);

#endif // !WORD_H
