#ifndef FUNCTION_H
#define FUNCTION_H

struct function
{
    char *name;
    struct ast *value;
    struct function *next; // Collisions are handled with a linked list
};

/**
 *  @brief Create a new function
 *  @param name The name of the function
 *  @param value The value of the function
 *  @return A pointer to the newly created function
 */
struct function *create_function(char *name, struct ast *value);

/**
 *  @brief Free the memory allocated for a function
 *  @param func The function to free
 */
void free_function(struct function *func);

#endif /* !FUNCTION_H */
