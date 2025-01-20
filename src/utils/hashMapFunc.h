#ifndef HASHMAPFUNC_H
#define HASHMAPFUNC_H

#include "function.h"

#define TABLE_SIZE 256

struct HashMapFunc
{
    struct function *map[TABLE_SIZE];
};

/**
 *  @brief Free the memory allocated for the hash table
 *  @param ht The hash table to free
 */
void free_hash_map_func(void);

/**
 *  @brief Hash a string
 *  @param str The string to hash
 *  @return The hash value
 */
void insertFunction(char *name, struct ast *value);

/**
 *  @brief Get a variable from the hash table
 *  @param ht The hash table
 *  @param name The name of the variable
 *  @return The variable
 */
struct ast *getFunction(char *name);

/**
 *  @brief Update the value of a variable in the hash table
 *  @param ht The hash table
 *  @param name The name of the variable to update
 *  @param value The new value of the variable
 */
void updateFunction(char *name, struct ast *value);

/**
 * Initialize the environment variables
 * @param argc The number of arguments
 * @param argv The arguments
 */
void func_init(int argc, char **argv);

#endif /* !HASHMAPFUNC_H */
