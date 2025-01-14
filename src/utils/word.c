#include "word.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/strings.h"

struct word *word_new(void)
{
    struct word *word = malloc(sizeof(struct word));
    if (!word)
        return NULL;
    string_init(&word->value);
    word->variables = NULL;
    word->var_length = 0;
    word->var_capacity = 0;
    return word;
}

void word_free(struct word *word)
{
    free(word->value.data);
    if (word->variables)
    {
        for (size_t i = 0; i < word->var_length; i++)
            string_free(&word->variables[i].name);
        free(word->variables);
    }
    free(word);
}

void word_push(struct word *word, char c)
{
    string_push(&word->value, c);
}

void word_push_variable(struct word *word, struct variable var)
{
    if (word->var_length >= word->var_capacity)
    {
        word->var_capacity = word->var_capacity + 16;
        word->variables = realloc(word->variables,
                                  word->var_capacity * sizeof(struct variable));
    }
    word->variables[word->var_length++] = var;
}

int word_equals(struct word *word1, struct word *word2)
{
    if (word1->value.length != word2->value.length)
        return 0;
    return strcmp(word1->value.data, word2->value.data) == 0;
}

char *word_eval(struct word *word, evaluator eval, void *data)
{
    struct string str;
    string_init(&str);
    size_t var_idx = 0;
    for (size_t pos = 0; pos < word->value.length; pos++)
    {
        if (word->variables && var_idx < word->var_length
            && pos == word->variables[var_idx].pos && eval)
        {
            char *var = eval(word->variables[var_idx].name.data, data);
            string_append(&str, var);
            var_idx++;
        }
        string_push(&str, word->value.data[pos]);
    }
    return str.data;
}
