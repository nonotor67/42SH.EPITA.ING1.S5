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

int word_equals(struct word *word1, struct word *word2)
{
    if (word1->value.length != word2->value.length)
        return 0;
    return strcmp(word1->value.data, word2->value.data) == 0;
}

int is_word_valid(struct word *word)
{
    int valid = 1;
    for (size_t i = 0; i < word->var_length; i++)
        if (word->variables[i].name.length == 0 && !word->variables[i].commands)
            valid = 0;
    return valid;
}

static void eval_subcommand(struct string *str, struct variable *var)
{
    // pipe the commands
    int pipefd[2];
    int status;
    if (pipe(pipefd) == -1)
        return;
    pid_t pid = fork();
    if (pid == 0) // child
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execute_node(var->commands);
        exit(0);
    }
    close(pipefd[1]);
    waitpid(pid, &status, 0);

    char buffer[1024];
    ssize_t read_bytes;
    while ((read_bytes = read(pipefd[0], buffer, 1024)) > 0)
        for (ssize_t i = 0; i < read_bytes; i++)
            string_push(str, buffer[i]);

    close(pipefd[0]);
}

static void eval_variable(struct string *str, struct variable *var)
{
    char *value = NULL;
    if (var->commands)
    {
        struct string data;
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
        int i = 0;
        while (value[i])
        {
            if (isspace(value[i]))
            {
                while (isspace(value[i]))
                    i++;
                if (value[i])
                    string_push(str, ' ');
                continue;
            }
            string_push(str, value[i++]);
        }
    }
    if (var->commands)
        free(value);
}

char *word_eval(struct word *word)
{
    struct string str;
    string_init(&str);
    size_t var_idx = 0;
    for (size_t pos = 0;; pos++)
    {
        if (word->variables && var_idx < word->var_length
            && pos == word->variables[var_idx].pos)
        {
            eval_variable(&str, &word->variables[var_idx]);
            var_idx++;
        }
        if (pos == word->value.length)
            break;
        string_push(&str, word->value.data[pos]);
    }
    return str.data;
}
