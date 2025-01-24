#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

struct vector
{
    size_t size;
    size_t capacity;
    void **data;
};

void vector_init(struct vector *vector);
struct vector *vector_new(void);
void vector_free(struct vector *vector);

void vector_push(struct vector *vector, void *element);

#endif //! VECTOR_H
