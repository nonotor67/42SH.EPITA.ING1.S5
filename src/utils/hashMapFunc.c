#include "hashMapFunc.h"

#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "utils.h"

static struct HashMapFunc global_functions = { { NULL } };

void free_hash_map_func(void)
{
    // Free the memory allocated for each function
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct function *func = global_functions.map[i];
        // Free the linked list
        while (func != NULL)
        {
            struct function *next = func->next;
            free_function(func);
            func = next;
        }
        global_functions.map[i] = NULL;
    }
}

static unsigned int hash(char *name)
{
    unsigned int hash = 0;
    for (int i = 0; name[i] != '\0'; i++)
    {
        hash = (hash << 4) + 2 * name[i];
    }

    return hash % TABLE_SIZE;
}

void insertFunction(char *name, struct ast *value)
{
    unsigned int index = hash(name);
    struct function *func = global_functions.map[index];
    // Search for the function in the linked list
    while (func != NULL)
    {
        if (strcmp(func->name, name) == 0)
        {
            // free the old value
            ast_free(func->value);
            // allocate memory for the new value and copy it
            func->value = ast_copy(value);
            return;
        }
        func = func->next;
    }

    // Create a new function
    struct function *new_func = create_function(name, value);
    new_func->next = global_functions.map[index];
    global_functions.map[index] = new_func;
}

struct ast *getFunction(char *name)
{
    unsigned int index = hash(name);
    struct function *func = global_functions.map[index];
    // Search for the function in the linked list
    while (func != NULL)
    {
        if (strcmp(func->name, name) == 0)
        {
            return func->value;
        }
        func = func->next;
    }

    return NULL;
}

void updateFunction(char *name, struct ast *value)
{
    unsigned int index = hash(name);
    struct function *func = global_functions.map[index];
    // Search for the function in the linked list
    while (func != NULL)
    {
        if (strcmp(func->name, name) == 0)
        {
            // free the old value
            ast_free(func->value);
            // allocate memory for the new value and copy it
            func->value = value;
            return;
        }
        func = func->next;
    }
}

struct HashMapFunc copy_hash_map_func(void)
{
    struct HashMapFunc copy = { { NULL } };
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct function *func = global_functions.map[i];
        while (func != NULL)
        {
            struct function *new_func =
                create_function(func->name, func->value);
            new_func->next = copy.map[i];
            copy.map[i] = new_func;

            func = func->next;
        }
    }
    return copy;
}

void setFunctionMap(struct HashMapFunc copy)
{
    global_functions = copy;
}
