#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_ON_NULL(ptr, message) \
    if (ptr == NULL)               \
    {                              \
        fprintf(stderr, message);  \
        exit(1);                   \
    }

struct reader* reader_new(const int argc, char *argv[])
{
    struct reader *reader = malloc(sizeof(struct reader));
    EXIT_ON_NULL(reader, "Error: Could not allocate reader\n");

    if (argc == 2)
    {
        reader->type = INPUT_FILE;
        reader->file = fopen(argv[1], "r");
        EXIT_ON_NULL(reader->file, "Error: Could not open file\n");
    }
    else if (argc == 3 && strcmp(argv[1], "-c") == 0)
    {
        reader->type = INPUT_STRING;
        reader->string = argv[2];
    }
    else if (argc == 1)
        reader->type = INPUT_STDIN;
    else
    {
        fprintf(stderr, "Usage: %s [-c command] [file]\n", argv[0]);
        exit(1);
    }

    reader->current = 0;
    return reader;
}

void reader_free(struct reader *reader)
{
    if (reader->type == INPUT_FILE)
        fclose(reader->file);
    free(reader);
}

int reader_is_stdin(const struct reader *reader)
{
    return reader->type == INPUT_STDIN;
}

int reader_next(struct reader *reader)
{
    reader->current++;
    if (reader->type == INPUT_STRING)
    {
        const char c = reader->string[reader->current];
        return c != '\0' ? c : EOF;
    }

    FILE *input = reader->type == INPUT_FILE ? reader->file : stdin;
    return fgetc(input);
}
