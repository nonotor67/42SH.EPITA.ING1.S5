#ifndef HASHMAP_H
#define HASHMAP_H

#include "variable.h"

#define TABLE_SIZE 256

struct HashMap
{
    struct Variable *map[TABLE_SIZE];
};

struct HashMap *create_hash_table();
void free_hash_map(struct HashMap *ht);

unsigned int hash(char *str);
void insertVariable(struct HashMap *ht, char *name, char *value);
struct Variable getVariable(struct HashMap *ht, char *name);
void removeVariable(struct HashMap *ht, char *name);
void updateVariable(struct HashMap *ht, char *name, char *value);

#endif /* !HASHMAP_H */
