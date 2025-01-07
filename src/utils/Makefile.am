lib_LIBRARIES = libutils.a

libutils_a_SOURCES =  \
  utils.c \
  utils.h \
  strings.c \
  strings.h

libutils_a_CPPFLAGS = -I$(top_srcdir)/src

libutils_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -g3 -fsanitize=address,undefined

noinst_LIBRARIES = libutils.a