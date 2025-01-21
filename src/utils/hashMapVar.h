#ifndef HASHMAPVAR_H
#define HASHMAPVAR_H

#include "variable.h"

#define TABLE_SIZE 256

struct HashMapVar
{
    struct Variable *map[TABLE_SIZE];
};

/**
 *  @brief Free the memory allocated for the hash table
 *  @param ht The hash table to free
 */
void free_hash_map_var(void);

/**
 *  @brief Hash a string
 *  @param str The string to hash
 *  @return The hash value
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
 *  @brief Copy the hash table
 *  @return The copy of the hash table
 */
struct HashMapVar copy_hash_map_var(void);

/**
 *  @brief Clear the hash table
 *  @param hash_map The hash table
 */
void clear_hash_map_var(struct HashMapVar *hash_map);

void setVariableMap(struct HashMapVar copy);

void env_init(int argc, char **argv);

#endif /* !HASHMAPVAR_H */
