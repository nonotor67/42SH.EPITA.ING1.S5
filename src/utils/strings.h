#ifndef STRINGS_H
#define STRINGS_H

#include <stdlib.h>

struct string
{
    char *data;
    size_t length;
    size_t capacity;
};

struct string_view
{
    const char *data;
    size_t length;
};

// initializes and allocates an empty string
struct string *string_new(void);
// initializes a string
void string_init(struct string *string);
// initializes and allocates a NULL-terminated string
struct string *string_from(const char *data);

// frees a string
void string_free(struct string *string);

// appends a character to a string
void string_push(struct string *string, char c);
// appends a string to a string
void string_append(struct string *string, const char *data);

// add more functions if needed

#endif /* !STRINGS_H */
