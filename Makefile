CFLAGS = -Wall -g

SRC = src/test.c

all:
	gcc test.c -o test

check: 
	gcc $(CFLAGS) tests/test.c $(SRC) -o test -lcriterion
	./test

clean:
	rm -rf test