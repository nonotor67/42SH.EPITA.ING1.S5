#ifndef VARIABLE_H
#define VARIABLE_H

struct Variable
{
    char *name;
    char *value;
    struct Variable *next; // Collisions are handled with a linked list
};

/*
    @brief Create a new variable
    @param name The name of the variable
    @param value The value of the variable
    @return A pointer to the newly created variable
 */
struct Variable *create_variable(char *name, char *value);
/*
    @brief Free the memory allocated for a variable
    @param var The variable to free
 */
void free_variable(struct Variable *var);

#endif /* !VARIABLE_H */
