CC= gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla -Isrc
DEBUG_FLAGS = -g -fsanitize=address,undefined -lcriterion
SRC = $(wildcard src/*/*.c src/*.c)
TEST = $(wildcard src/[!main]*/*.c src/[!main]*.c test/*.c)
OBJ = $(SRC:.c=.o)

all: 42sh

42sh: $(OBJ)
	$(CC) $(OBJ) -o 42sh

debug:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SRC) -o debug

check: 
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(TEST) -o testsuite
	./testsuite

format:
	./tests/clang-check/clang-check.sh

clean:
	rm -f 42sh debug testsuite
	rm -f $(wildcard $(OBJ))
