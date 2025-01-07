check_PROGRAMS = unit_tests

unit_tests_SOURCES = test_ast.c

unit_tests_CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
unit_tests_LDADD = -lcriterion -L$(abs_top_builddir)/src -last -llexer -lparser -lexecution -lio -lutils
unit_tests_CPPFLAGS = -I$(abs_top_srcdir)/src

EXTRA_DIST = $(unit_tests_SOURCES)
