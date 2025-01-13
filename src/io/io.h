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
    INPUT_STDIN
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
struct reader *reader_new(int argc, char *argv[]);
// frees a reader struct
void reader_free(struct reader *reader);
// returns 1 if the input is stdin, 0 otherwise
int reader_is_stdin(const struct reader *reader);

/*
 Returns the next character from the input.
 Returns EOF if the end of the input is reached.
*/
int reader_next(struct reader *reader);

#endif /* !IO_H */
