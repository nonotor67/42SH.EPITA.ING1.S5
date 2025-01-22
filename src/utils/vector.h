#ifndef INC_42SH_VECTOR_H
#define INC_42SH_VECTOR_H

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

#endif //!INC_42SH_VECTOR_H
