CFLAGS = -Wall -g

SRC = src/test.c

all:
	gcc test.c -o test

check: 
	gcc $(CFLAGS) tests/test.c $(SRC) -o test -lcriterion
	./test

format:
	./tests/clang-check/clang-check.sh

clean:
	rm -rf test