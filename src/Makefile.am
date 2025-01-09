# define the subdirectories
SUBDIRS = utils io lexer parser ast execution

bin_PROGRAMS = 42sh

42sh_SOURCES = 42sh.c

42sh_CPPFLAGS = -I%D%

42sh_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

42sh_LDADD =  \
       ast/libast.a \
       execution/libexecution.a\
       io/libio.a\
       lexer/liblexer.a \
       parser/libparser.a \
       utils/libutils.a