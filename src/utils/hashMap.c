#include <stddef.h>
#include <string.h>

#include "utils.h"

struct HashMap *create_hash_table()
{
    struct HashMap *ht = xmalloc(sizeof(struct HashMap));
    // Initialize the hash table with NULL values
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        ht->map[i] = NULL;
    }

    return ht;
}

void free_hash_map(struct HashMap *ht)
{
    // Free the memory allocated for each variable
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct Variable *var = ht->map[i];
        // Free the linked list
        while (var != NULL)
        {
            struct Variable *next = var->next;
            free_variable(var);
            var = next;
        }
    }
    free(ht);
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

void insertVariable(struct HashMap *ht, char *name, char *value)
{
    unsigned int index = hash(name);
    struct Variable *var = create_variable(name, value);
    var->next = ht->map[index];
    ht->map[index] = var;
}

struct Variable getVariable(struct HashMap *ht, char *name)
{
    unsigned int index = hash(name);
    struct Variable *var = ht->map[index];
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

void updateVariable(struct HashMap *ht, char *name, char *value)
{
    unsigned int index = hash(name);
    struct Variable *var = ht->map[index];
    // Search for the variable in the linked list
    while (var != NULL)
    {
        if (strcmp(var->name, name) == 0)
        {
            // free the old value
            free(var->value);
            // allocate memory for the new value and copy it
            var->value = strcpy(xmalloc(strlen(value) + 1), value);
            return;
        }
        var = var->next;
    }
}
