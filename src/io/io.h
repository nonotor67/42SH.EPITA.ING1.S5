#ifndef IO_H
#define IO_H

#define BUFFER_SIZE 1024

char *read_input(int argc, char *argv[]);

char *read_from_string(char *argv[]);

char *read_from_file(char *filename);

char *read_from_stdin();

#endif /* ! IO_H */
