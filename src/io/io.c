#include "io.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    reader->input.file.fd = open(filename, O_RDONLY);
    if (reader->input.file.fd == -1)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }
    reader->input.file.size = 0;
    reader->input.file.index = 0;
    return reader;
}

struct reader *reader_from_stdin(void)
{
    struct reader *reader = malloc(sizeof(struct reader));
    EXIT_ON_NULL(reader, "Error: Could not allocate reader\n");

    reader->status = READER_OK;
    reader->type = INPUT_FILE;
    reader->input.file.fd = STDIN_FILENO;
    reader->input.file.size = 0;
    reader->input.file.index = 0;
    return reader;
}

void reader_free(struct reader *reader)
{
    if (reader->type == INPUT_FILE)
        close(reader->input.file.fd);
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
    {
        struct reader_file *file = &reader->input.file;
        if (file->index == file->size)
        {
            file->size = read(file->fd, file->buffer, BUFFER_SIZE);
            if (file->size == 0)
                res = EOF;
            file->index = 0;
        }
        if (res != EOF)
            res = (int)file->buffer[file->index++];
    }

    if (res == EOF)
        reader->status = READER_EOF;
    return res;
}
