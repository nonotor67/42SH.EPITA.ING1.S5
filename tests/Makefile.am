# Exécutable pour les tests
check_PROGRAMS = unit_tests

# Sources des tests
unit_tests_SOURCES = test_io.c test_execution.c tests_parser.c test_ast_execution.c test_lexer.c test_utils.c

# Flags de compilation et de lien
unit_tests_CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
unit_tests_LDADD = -lcriterion -L$(abs_top_builddir)/src/io -lio -L$(abs_top_builddir)/src/execution -lexecution \
	-L$(abs_top_builddir)/src/parser -lparser -L$(abs_top_builddir)/src/lexer -llexer \
	-L$(abs_top_builddir)/src/ast -last -L$(abs_top_builddir)/src/utils -lutils
unit_tests_CPPFLAGS = -I$(abs_top_srcdir)/src

check-local: $(check_PROGRAMS)
	$(MAKE) $(check_PROGRAMS) || true
	./run_tests.sh

test: $(check_PROGRAMS)
	$(MAKE) $(check_PROGRAMS)
	BIN_PATH="../src/42sh" COVERAGE="yes" ./run_tests.sh
