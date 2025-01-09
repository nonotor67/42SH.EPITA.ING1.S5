# Exécutable pour les tests
check_PROGRAMS = unit_tests

# Sources des tests
unit_tests_SOURCES = test_io.c test_execution.c tests_parser.c test_lexer.c

# Flags de compilation et de lien
unit_tests_CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -g3 -fsanitize=address,undefined
unit_tests_LDADD = -lcriterion -L$(abs_top_builddir)/src/io -lio -L$(abs_top_builddir)/src/execution -lexecution \
	-L$(abs_top_builddir)/src/parser -lparser -L$(abs_top_builddir)/src/lexer -llexer \
	-L$(abs_top_builddir)/src/ast -last -L$(abs_top_builddir)/src/utils -lutils
unit_tests_CPPFLAGS = -I$(abs_top_srcdir)/src

check: $(check_PROGRAMS)
	$(MAKE) $(check_PROGRAMS)
	./unit_tests