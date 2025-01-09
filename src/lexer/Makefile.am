lib_LIBRARIES = liblexer.a

liblexer_a_SOURCES =  \
  lexer.c \
  lexer.h \
  token.h \
  token.c

liblexer_a_CPPFLAGS = -I$(top_srcdir)/src

liblexer_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -g3 -fsanitize=address,undefined

noinst_LIBRARIES = liblexer.a