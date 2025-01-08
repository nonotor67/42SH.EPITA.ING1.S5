CC= gcc
CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -I../src -g -fsanitize=address,undefined --coverage -fPIC
LDFLAGS = --coverage
LDLIBS = -lgcov
SRC = $(wildcard ../src/*/*.c ../src/*.c)
TEST = $(wildcard ../src/[!42sh]*/*.c ../src/[!42sh]*.c ../tests/*.c)
OBJ = $(SRC:.c=.o)

all: 42sh

42sh: $(OBJ)
	$(CC) $(OBJ) -o 42sh

debug:
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SRC) -o debug

check:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(TEST) -lcriterion -o testsuite

format:
	./tests/clang-check/clang-check.sh

clean:
	rm -f 42sh debug testsuite
	rm -f $(wildcard $(OBJ))
	find -name '*.gc*' -delete