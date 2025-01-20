lib_LIBRARIES = libutils.a

libutils_a_SOURCES =  \
  utils.c \
  utils.h \
  strings.c \
  strings.h \
  xmalloc.h \
  xmalloc.c \
  hashMapVar.h \
  hashMapVar.c \
  variable.h \
  variable.c \
  word.h \
  word.c


libutils_a_CPPFLAGS = -I$(top_srcdir)/src

libutils_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libutils.a
