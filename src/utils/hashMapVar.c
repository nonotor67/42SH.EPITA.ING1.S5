#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"

struct HashMapVar global_variables = { { NULL } };

void free_hash_map_var(void)
{
    // Free the memory allocated for each variable
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct Variable *var = global_variables.map[i];
        // Free the linked list
        while (var != NULL)
        {
            struct Variable *next = var->next;
            free_variable(var);
            var = next;
        }
        global_variables.map[i] = NULL;
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

static void updateVariable(char *name, char *value)
{
    unsigned int index = hash(name);
    struct Variable *var = global_variables.map[index];
    // Search for the variable in the linked list
    while (var != NULL)
    {
        if (strcmp(var->name, name) == 0)
        {
            // free the old value
            free(var->value);
            // allocate memory for the new value and copy it
            var->value = strcpy(xmalloc(strlen(value) + 1), value);
        }
        var = var->next;
    }
}

void insertVariable(char *name, char *value)
{
    if (getVariable(name).name != NULL)
    {
        updateVariable(name, value);
        return;
    }
    unsigned int index = hash(name);
    struct Variable *var = create_variable(name, value);
    var->next = global_variables.map[index];
    global_variables.map[index] = var;
}

void insertBlindVariable(char *name, char *value)
{
    unsigned int index = hash(name);
    struct Variable *var = create_variable(name, value);
    var->next = global_variables.map[index];
    global_variables.map[index] = var;
}

void removeVariable(char *name)
{
    unsigned int index = hash(name);
    struct Variable *var = global_variables.map[index];
    struct Variable *prev = NULL;
    // Search for the variable in the linked list
    while (var != NULL)
    {
        if (strcmp(var->name, name) == 0)
        {
            if (prev == NULL)
            {
                global_variables.map[index] = var->next;
            }
            else
            {
                prev->next = var->next;
            }
            free_variable(var);
            return;
        }
        prev = var;
        var = var->next;
    }
}

static char rand_buf[16];

struct Variable getVariable(char *name)
{
    if (strcmp(name, "RANDOM") == 0)
    {
        sprintf(rand_buf, "%d", rand() % 32767);
        return (struct Variable){ "RANDOM", rand_buf, NULL };
    }

    unsigned int index = hash(name);
    struct Variable *var = global_variables.map[index];
    // Search for the variable in the linked list
    while (var != NULL)
    {
        if (strcmp(name, var->name) == 0)
        {
            return *var;
        }
        var = var->next;
    }

    char *env_value = getenv(name);
    if (env_value != NULL)
    {
        return (struct Variable){ name, env_value, NULL };
    }

    return (struct Variable){ NULL, NULL, NULL };
}

struct HashMapVar copy_hash_map_var(void)
{
    struct HashMapVar copy = { { NULL } };
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        struct Variable *var = global_variables.map[i];
        while (var != NULL)
        {
            struct Variable *new_var = create_variable(var->name, var->value);
            new_var->next = copy.map[i];
            copy.map[i] = new_var;

            var = var->next;
        }
    }
    return copy;
}

void setVariableMap(struct HashMapVar copy)
{
    global_variables = copy;
}

/**
 * Initialize the environment variables
 * @param argc The number of arguments
 * @param argv The arguments
 */
void env_init(int argc, char **argv)
{
    char *argument_list = xmalloc(1024);
    unsigned int argument_list_size = 1024;
    unsigned int actual_list_size = 0;

    for (int i = 1; i < argc; i++)
    {
        if (actual_list_size + strlen(argv[i]) + 1 >= argument_list_size)
        {
            argument_list_size *= 2;
            argument_list = xrealloc(argument_list, argument_list_size);
        }
        strcpy(argument_list + actual_list_size, argv[i]);
        actual_list_size += strlen(argv[i]);
        argument_list[actual_list_size] = ' ';
        actual_list_size++;
        char *int_tmp = xmalloc(256);
        insertBlindVariable(my_itoa(i, int_tmp), argv[i]);
        free(int_tmp);
    }

    insertBlindVariable("0", argv[0]);

    if (actual_list_size > 0)
    {
        argument_list[actual_list_size - 1] = '\0';
        insertBlindVariable("@", argument_list);
        insertBlindVariable("*", argument_list);
    }
    free(argument_list);

    // InsertVariables allocate a new string, so we can free or use the old one
    char *int_tmp = xmalloc(256);
    insertBlindVariable("#", my_itoa(argc - 1, int_tmp));
    insertBlindVariable("$", my_itoa(getpid(), int_tmp));
    insertBlindVariable("UID", my_itoa(getuid(), int_tmp));
    free(int_tmp);

    insertBlindVariable("?", "0");

    char *pwd = getcwd(NULL, 0);
    if (pwd)
    {
        insertBlindVariable("PWD", pwd);
        free(pwd);
    }
    insertBlindVariable("OLDPWD", "");
    insertBlindVariable("IFS", " \t\n");
}
