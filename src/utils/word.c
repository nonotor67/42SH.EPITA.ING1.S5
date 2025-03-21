#include "word.h"

#include <ast/ast.h>
#include <ctype.h>
#include <execution/execution.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils/utils.h"
#include "vector.h"

struct word *word_new(void)
{
    struct word *word = malloc(sizeof(struct word));
    if (!word)
        return NULL;
    string_init(&word->value);
    word->variables = NULL;
    word->var_length = 0;
    word->var_capacity = 0;
    word->has_escaped = 0;
    word->valid_assignment = 1; // default to true
    return word;
}

void word_copy(struct word *word, struct word *copy)
{
    copy->has_escaped = word->has_escaped;
    copy->valid_assignment = word->valid_assignment;
    copy->var_length = 0;
    copy->var_capacity = 0;
    if (word->variables)
    {
        for (size_t i = 0; i < word->var_length; i++)
        {
            struct variable var;
            var.pos = word->variables[i].pos;
            string_init(&var.name);
            size_t j = 0;
            while (word->variables[i].name.data[j])
            {
                string_push(&var.name, word->variables[i].name.data[j]);
                j++;
            }
            var.commands = ast_copy(word->variables[i].commands);
            var.is_quoted = word->variables[i].is_quoted;
            word_push_variable(copy, var);
        }
    }
    size_t i = 0;
    while (word->value.data[i])
    {
        word_push(copy, word->value.data[copy->value.length]);
        i++;
    }
}

void word_free(struct word *word)
{
    free(word->value.data);
    if (word->variables)
    {
        for (size_t i = 0; i < word->var_length; i++)
        {
            struct variable *var = &word->variables[i];
            if (var->name.data)
                free(var->name.data);
            if (var->commands)
                ast_free(var->commands);
        }
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

int is_word_valid(struct word *word)
{
    int valid = 1;
    for (size_t i = 0; i < word->var_length; i++)
        if (word->variables[i].name.length == 0 && !word->variables[i].commands)
            valid = 0;
    return valid;
}

void trim_subcommand(struct string *str)
{
    int i = (int)str->length - 1;
    while (i >= 0 && str->data[i] == '\n')
        i--;
    str->length = i + 1;
    str->data[str->length] = '\0';
}

static void eval_subcommand(struct string *str, struct variable *var)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return;
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execution(var->commands);
        exit(0);
    }
    waitpid(pid, NULL, 0);
    close(pipefd[1]);
    char buffer[1024];
    ssize_t read_bytes;
    while ((read_bytes = read(pipefd[0], buffer, 1024)) > 0)
        for (ssize_t i = 0; i < read_bytes; i++)
            string_push(str, buffer[i]);
    trim_subcommand(str);
    close(pipefd[0]);
}

static void eval_variable(struct vector *vect, struct string *str,
                          struct variable *var)
{
    char *value = NULL;
    if (var->commands)
    {
        struct string data;
        string_init(&data);
        eval_subcommand(&data, var);
        value = data.data;
    }
    else
    {
        struct Variable retrieved = getVariable(var->name.data);
        if (retrieved.value)
            value = retrieved.value;
    }

    if (value)
    {
        if (var->is_quoted)
            string_append(str, value);
        else
        {
            int i = 0;
            while (value[i])
            {
                if (isspace(value[i]))
                {
                    while (isspace(value[i]))
                        i++;
                    struct string copy;
                    string_init(&copy);
                    string_append(&copy, str->data);
                    vector_push(vect, copy.data);
                    str->length = 0;
                }
                if (value[i])
                    string_push(str, value[i++]);
            }
        }
    }
    if (var->commands)
        free(value);
}

char **word_eval(struct word *word)
{
    struct vector words;
    vector_init(&words);
    struct string curr_str;
    string_init(&curr_str);

    size_t var_idx = 0;
    for (size_t pos = 0;; pos++)
    {
        while (word->variables && var_idx < word->var_length
               && pos == word->variables[var_idx].pos)
        {
            eval_variable(&words, &curr_str, &word->variables[var_idx]);
            var_idx++;
        }
        if (pos == word->value.length)
            break;
        string_push(&curr_str, word->value.data[pos]);
    }
    if (!word->has_escaped && curr_str.length == 0)
        free(curr_str.data);
    else
        vector_push(&words, curr_str.data);

    return (char **)words.data;
}
