CC= gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -Isrc
SRC = $(wildcard src/*/*.c src/*.c)
OBJ = $(SRC:.c=.o)

all: 42sh

42sh: $(OBJ)
	$(CC) $(OBJ) -o 42sh

debug: $(SRC)
	$(CC) $(CFLAGS) -g -fsanitize=address,undefined $(SRC) -o debug

clean:
	rm -f $(wildcard $(OBJ)
	rm -f $(wildcard src/*/*.gc* src/*.gc*)
