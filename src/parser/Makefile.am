lib_LIBRARIES = libparser.a

libparser_a_SOURCES =  \
  parser.c \
  parser_base.c \
  parser.h \
  parser_base.h \
  parser_redir.h \
  parser_struct.h

libparser_a_CPPFLAGS = -I$(top_srcdir)/src

libparser_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libparser.a