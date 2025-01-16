#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_ON_NULL(ptr, message)                                             \
    if (ptr == NULL)                                                           \
    {                                                                          \
        fprintf(stderr, message);                                              \
        exit(1);                                                               \
    }

struct reader *reader_from_string(const char *string)
{
    struct reader *reader = malloc(sizeof(struct reader));
    EXIT_ON_NULL(reader, "Error: Could not allocate reader\n");

    reader->status = READER_OK;
    reader->type = INPUT_STRING;
    reader->input.string = (char *)string;
    reader->current = 0;
    return reader;
}

struct reader *reader_from_file(const char *filename)
{
    struct reader *reader = malloc(sizeof(struct reader));
    EXIT_ON_NULL(reader, "Error: Could not allocate reader\n");

    reader->status = READER_OK;
    reader->type = INPUT_FILE;
    reader->input.file = fopen(filename, "r");
    EXIT_ON_NULL(reader->input.file, "Error: Could not open file\n");

    return reader;
}

struct reader *reader_from_stdin(void)
{
    struct reader *reader = malloc(sizeof(struct reader));
    EXIT_ON_NULL(reader, "Error: Could not allocate reader\n");

    reader->status = READER_OK;
    reader->type = INPUT_FILE;
    reader->input.file = stdin;
    reader->current = 0;
    return reader;
}

void reader_free(struct reader *reader)
{
    if (reader->type == INPUT_FILE)
        fclose(reader->input.file);
    free(reader);
}

int reader_next(struct reader *reader)
{
    if (reader->status == READER_EOF)
    {
        fprintf(stderr, "Error: Reader is at EOF\n");
        exit(1);
    }
    int res = 0;
    if (reader->type == INPUT_STRING)
    {
        const char c = reader->input.string[reader->current++];
        // replace the null terminator with EOF
        res = c != '\0' ? c : EOF;
    }
    else
        res = fgetc(reader->input.file);

    if (res == EOF)
        reader->status = READER_EOF;
    return res;
}
