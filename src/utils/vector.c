#include "vector.h"

#include <stdlib.h>

void vector_init(struct vector *vector)
{
    vector->size = 0;
    vector->capacity = 0;
    vector->data = NULL;
}

struct vector *vector_new(void)
{
    struct vector *vector = malloc(sizeof(struct vector));
    if (!vector)
        return NULL;
    vector_init(vector);
    return vector;
}

void vector_free(struct vector *vector)
{
    free(vector->data);
    free(vector);
}

void vector_push(struct vector *vector, void *element)
{
    if (vector->size + 1 >= vector->capacity)
    {
        vector->capacity = vector->capacity == 0 ? 16 : vector->capacity * 2;
        vector->data = realloc(vector->data, vector->capacity * sizeof(void *));
    }
    vector->data[vector->size++] = element;
    vector->data[vector->size] = NULL;
}
