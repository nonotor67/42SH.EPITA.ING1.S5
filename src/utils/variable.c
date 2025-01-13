#include <string.h>

#include "utils.h"

struct Variable *create_variable(char *name, char *value)
{
    struct Variable *var = xmalloc(sizeof(struct Variable));
    var->name = strcpy(xmalloc(strlen(name) + 1), name);
    var->value = strcpy(xmalloc(strlen(value) + 1), value);
    var->next = NULL;

    return var;
}

void free_variable(struct Variable *var)
{
    free(var->name);
    free(var->value);
    free(var);
}
