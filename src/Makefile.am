# define the subdirectories
SUBDIRS = utils io lexer parser ast execution

bin_PROGRAMS = 42sh

42sh_SOURCES = 42sh.c

42sh_CPPFLAGS = -I$(top_srcdir)/src

42sh_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -fsanitize=address,undefined
42sh_LDFLAGS = -fsanitize=address,undefined

42sh_LDADD = -L$(abs_top_builddir)/src/io -lio -L$(abs_top_builddir)/src/execution -lexecution \
             	-L$(abs_top_builddir)/src/parser -lparser -L$(abs_top_builddir)/src/lexer -llexer \
             	-L$(abs_top_builddir)/src/ast -last -L$(abs_top_builddir)/src/utils -lutils