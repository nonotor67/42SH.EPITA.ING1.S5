#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_input(int argc, char *argv[])
{
    if (argc == 2)
    {
        return read_from_file(argv[1]);
    }
    else if (argc == 3 && strcmp(argv[1], "-c") == 0)
    {
        return read_from_string(argv);
    }
    else if (argc == 1)
    {
        return read_from_stdin();
    }
    else
    {
        fprintf(stderr, "Usage: %s [-c command] [file]\n", argv[0]);
        exit(1);
    }
}

// strdup is not standard C, to avoid portability issues, we implement our own
static char *my_strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *dup = malloc(len);
    if (dup == NULL)
        return NULL;
    memcpy(dup, s, len);
    return dup;
}

char *read_from_string(char *argv[])
{
    // Strdup to allow freeing the memory
    return my_strdup(argv[2]);
}

char *read_from_file(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    size_t total_size = BUFFER_SIZE;
    size_t current_size = 0;
    char *input = malloc(total_size);
    if (input == NULL)
    {
        fprintf(stderr, "Error: Could not allocate input\n");
        exit(1);
    }
    input[0] = '\0';

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file) != NULL)
    {
        size_t buffer_len = strlen(buffer);
        if (current_size + buffer_len + 1 > total_size)
        {
            total_size *= 2;
            input = realloc(input, total_size);
            if (input == NULL)
            {
                fprintf(stderr, "Error: Could not reallocate input\n");
                exit(1);
            }
        }
        strcat(input, buffer);
        current_size += buffer_len;
    }

    fclose(file);
    return input;
}

char *read_from_stdin()
{
    size_t total_size = BUFFER_SIZE;
    size_t current_size = 0;
    char *input = malloc(total_size);
    if (input == NULL)
    {
        fprintf(stderr, "Error: Could not allocate input\n");
        exit(1);
    }
    input[0] = '\0';

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL)
    {
        size_t buffer_len = strlen(buffer);
        if (current_size + buffer_len + 1 > total_size)
        {
            total_size *= 2;
            input = realloc(input, total_size);
            if (input == NULL)
            {
                fprintf(stderr, "Error: Could not reallocate input\n");
                exit(1);
            }
        }
        strcat(input, buffer);
        current_size += buffer_len;
    }

    return input;
}
