 make clean
 ./configure CFLAGS="-Wall -Wextra -Werror -Wvla -pedantic -std=c99 -g3 -fsanitize=address" LDFLAGS="-fsanitize=address"
 make
