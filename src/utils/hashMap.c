#include <string.h>

#include "utils.h"

struct HashMap *create_hash_table()
{
    struct HashMap *ht = xmalloc(sizeof(struct HashMap));
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        ht->map[i] = NULL;
    }

    return ht;
}

void free_hash_map(struct HashMap *ht)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct Variable *var = ht->map[i];
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

void insert(struct HashMap *ht, char *name, char *value)
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

void removeVariable(struct HashMap *ht, char *name)
{
    unsigned int index = hash(name);
    struct Variable *var = ht->map[index];
    struct Variable *prev = NULL;
    while (var != NULL)
    {
        if (strcmp(var->name, name) == 0)
        {
            if (prev == NULL)
            {
                ht->map[index] = var->next;
            }
            else
            {
                prev->next = var->next;
            }
            free_variable(var);
            return;
        }
        prev = var;
        var = var->next;
    }
}
