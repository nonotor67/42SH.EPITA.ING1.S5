lib_LIBRARIES = libparser.a

libparser_a_SOURCES =  \
  parser.c \
  parser_command.c \
  parser.h \
  parser_command.h \
  parser_operator.h \
  parser_operator.c \
  parser_struct.h \
  parser_struct.c \
  parser_shell_command.h \
  parser_shell_command.c

libparser_a_CPPFLAGS = -I$(top_srcdir)/src

libparser_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libparser.a