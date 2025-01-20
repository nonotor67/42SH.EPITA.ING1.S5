#include <stdlib.h>

#include "HashMapFunc.h"
#include "HashMapVar.h"
#include <string.h>

static struct HashMapFunc global_functions = { { NULL } };

void free_hash_map_func(void)
{
    // Free the memory allocated for each function
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct ast *func = global_functions.map[i];
        // Free the linked list
        while (func != NULL)
        {
            struct ast *next = func->next;
            ast_free(func);
            func = next;
        }
        global_functions.map[i] = NULL;
    }
}

void insertFunction(char *name, struct ast *value)
{
    if (getFunction(name) != NULL)
    {
        updateFunction(name, value);
        return;
    }
    unsigned int index = hash(name);
    value->next = global_functions.map[index];
    global_functions.map[index] = value;
}

struct ast *getFunction(char *name)
{
    unsigned int index = hash(name);
    struct ast *func = global_functions.map[index];
    // Search for the function in the linked list
    while (func != NULL)
    {
        if (strcmp(func->name, name) == 0)
        {
            return func;
        }
        func = func->next;
    }

    return NULL;
}

void updateFunction(char *name, struct ast *value)
{
    unsigned int index = hash(name);
    struct ast *func = global_functions.map[index];
    // Search for the function in the linked list
    while (func != NULL)
    {
        if (strcmp(func->name, name) == 0)
        {
            ast_free(func);
            global_functions.map[index] = value;
            return;
        }
        func = func->next;
    }
}
