#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void *xmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    return ptr;
}

void *xcalloc(size_t nmemb, size_t size)
{
    void *ptr = calloc(nmemb, size);
    if (ptr == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL)
    {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    return new_ptr;
}
