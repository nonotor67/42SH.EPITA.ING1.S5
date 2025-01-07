#include "strings.h"

#include <string.h>

#define BUFFER_STEP 16

struct string *string_new(void)
{
    struct string *string = malloc(sizeof(struct string));
    if (string == NULL)
        return NULL;

    string->data = malloc(BUFFER_STEP);
    if (string->data == NULL)
    {
        free(string);
        return NULL;
    }

    string->length = 0;
    string->capacity = BUFFER_STEP;
    string->data[0] = '\0';
    return string;
}

struct string *string_from(const char *data)
{
    struct string *string = malloc(sizeof(struct string));
    if (string == NULL)
        return NULL;

    string->length = strlen(data);
    string->capacity = string->length + 1;
    string->data = malloc(string->capacity);
    if (string->data == NULL)
    {
        free(string);
        return NULL;
    }

    strcpy(string->data, data);
    return string;
}

void string_free(struct string *string)
{
    free(string->data);
    free(string);
}

void string_push(struct string *string, char c)
{
    if (string->length + 1 >= string->capacity)
    {
        string->capacity += BUFFER_STEP;
        string->data = realloc(string->data, string->capacity);
    }

    string->data[string->length++] = c;
    string->data[string->length] = '\0';
}

void string_append(struct string *string, const char *data)
{
    const size_t data_length = strlen(data);
    if (string->length + data_length + 1 >= string->capacity)
    {
        string->capacity = string->length + data_length + 1 + BUFFER_STEP;
        string->data = realloc(string->data, string->capacity);
    }

    strcpy(string->data + string->length, data);
    string->length += data_length;
}