#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static struct HashMap global_variables = { { NULL } };

void free_hash_map(void)
{
    // Free the memory allocated for each variable
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct Variable *var = global_variables.map[i];
        // Free the linked list
        while (var != NULL)
        {
            struct Variable *next = var->next;
            free_variable(var);
            var = next;
        }
        global_variables.map[i] = NULL;
    }
}

unsigned int hash(char *name)
{
    unsigned int hash = 0;
    for (int i = 0; name[i] != '\0'; i++)
    {
        hash = (hash << 4) + 2 * name[i];
    }

    return hash % TABLE_SIZE;
}

void insertVariable(char *name, char *value)
{
    if (getVariable(name).name != NULL)
    {
        updateVariable(name, value);
        return;
    }
    unsigned int index = hash(name);
    struct Variable *var = create_variable(name, value);
    var->next = global_variables.map[index];
    global_variables.map[index] = var;
}

static char rand_buf[16];

struct Variable getVariable(char *name)
{
    if (strcmp(name, "RANDOM") == 0)
    {
        sprintf(rand_buf, "%d", rand() % 32767);
        return (struct Variable){ "RANDOM", rand_buf, NULL };
    }

    unsigned int index = hash(name);
    struct Variable *var = global_variables.map[index];
    // Search for the variable in the linked list
    while (var != NULL)
    {
        if (strcmp(var->name, name) == 0)
        {
            return *var;
        }
        var = var->next;
    }

    return (struct Variable){ NULL, NULL, NULL };
}

void updateVariable(char *name, char *value)
{
    unsigned int index = hash(name);
    struct Variable *var = global_variables.map[index];
    // Search for the variable in the linked list
    while (var != NULL)
    {
        if (strcmp(var->name, name) == 0)
        {
            // free the old value
            free(var->value);
            // allocate memory for the new value and copy it
            var->value = strcpy(xmalloc(strlen(value) + 1), value);
        }
        var = var->next;
    }
}
