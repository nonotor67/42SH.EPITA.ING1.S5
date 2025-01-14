#ifndef HASHMAP_H
#define HASHMAP_H

#include "variable.h"

#define TABLE_SIZE 256

struct HashMap
{
    struct Variable *map[TABLE_SIZE];
};

// global variable to store the variables
extern struct HashMap *global_variables;

/*
    @brief Create a new hash table
    @return A pointer to the newly created hash table
    @note The hash table is initialized with NULL values
 */
struct HashMap *create_hash_table();
/*
    @brief Free the memory allocated for the hash table
    @param ht The hash table to free
 */
void free_hash_map(struct HashMap *ht);

/*
    @brief Hash a string
    @param str The string to hash
    @return The hash value
 */
unsigned int hash(char *str);
/*
    @brief Insert a new variable in the hash table
    @param ht The hash table
    @param name The name of the variable
    @param value The value of the variable
 */
void insertVariable(struct HashMap *ht, char *name, char *value);
/*
    @brief Get a variable from the hash table
    @param ht The hash table
    @param name The name of the variable
    @return The variable
 */
struct Variable getVariable(struct HashMap *ht, char *name);
/*
    @brief Update the value of a variable in the hash table
    @param ht The hash table
    @param name The name of the variable to update
    @param value The new value of the variable
 */
void updateVariable(struct HashMap *ht, char *name, char *value);

#endif /* !HASHMAP_H */
