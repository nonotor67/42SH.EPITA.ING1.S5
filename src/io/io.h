#ifndef IO_H
#define IO_H

#include <stdio.h>

enum reader_status
{
    READER_OK,
    READER_EOF
};

enum input_type
{
    INPUT_STRING,
    INPUT_FILE,
};

struct reader
{
    enum reader_status status;
    enum input_type type;
    int current;
    union
    {
        FILE *file;
        char *string;
    } input;
};

// initializes a reader struct
struct reader *reader_from_string(const char *string);
struct reader *reader_from_file(const char *filename);
struct reader *reader_from_stdin(void);

// frees a reader struct
void reader_free(struct reader *reader);

/*
 Returns the next character from the input.
 Returns EOF if the end of the input is reached.
*/
int reader_next(struct reader *reader);

#endif /* !IO_H */
