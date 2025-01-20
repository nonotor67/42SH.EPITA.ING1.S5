#ifndef HASHMAP_H
#define HASHMAP_H

#include "variable.h"

#define TABLE_SIZE 256

struct HashMap
{
    struct Variable *map[TABLE_SIZE];
};

/**
 *  @brief Free the memory allocated for the hash table
 *  @param ht The hash table to free
 */
void free_hash_map(void);

/**
 *  @brief Hash a string
 *  @param str The string to hash
 *  @return The hash value
 */
unsigned int hash(char *str);
/**
 *  @brief Insert a new variable in the hash table
 *  @param ht The hash table
 *  @param name The name of the variable
 *  @param value The value of the variable
 */
void insertVariable(char *name, char *value);
/**
 *  @brief Get a variable from the hash table
 *  @param ht The hash table
 *  @param name The name of the variable
 *  @return The variable
 */
struct Variable getVariable(char *name);
/**
 *  @brief Update the value of a variable in the hash table
 *  @param ht The hash table
 *  @param name The name of the variable to update
 *  @param value The new value of the variable
 */
void updateVariable(char *name, char *value);

/**
 * Initialize the environment variables
 * @param argc The number of arguments
 * @param argv The arguments
 */
void env_init(int argc, char **argv);

#endif /* !HASHMAP_H */
