CC= gcc
CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -Isrc
DEBUG_FLAGS = -g -fsanitize=address,undefined
SRC = $(wildcard src/*/*.c src/*.c)
TEST = $(wildcard src/[!main]*/*.c src/[!main]*.c tests/*.c)
OBJ = $(SRC:.c=.o)

all: 42sh

42sh: $(OBJ)
	$(CC) $(OBJ) -o 42sh

debug:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SRC) -o debug

check: 
	$(CC) $(CFLAGS) $(TEST) --coverage -fPIC -lgcov -lcriterion -o testsuite
	GCC_COLLECT_GCOV=1 ./testsuite

format:
	./tests/clang-check/clang-check.sh

clean:
	rm -f 42sh debug testsuite
	rm -f $(wildcard $(OBJ))
	find -name '*.gc*' -delete
