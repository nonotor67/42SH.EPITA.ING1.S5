#ifndef VARIABLE_H
#define VARIABLE_H

struct Variable
{
    char *name;
    char *value;
    struct Variable *next; // Collisions are handled with a linked list
};

struct Variable *create_variable(char *name, char *value);
void free_variable(struct Variable *var);

#endif /* !VARIABLE_H */
