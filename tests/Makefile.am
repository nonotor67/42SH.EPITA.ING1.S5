# Exécutable pour les tests
check_PROGRAMS = unit_tests

# Sources des tests
unit_tests_SOURCES = test_io.c test_execution.c

# Flags de compilation et de lien
unit_tests_CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
unit_tests_LDADD = -lcriterion -L$(abs_top_builddir)/src/io -lio -L$(abs_top_builddir)/src/execution -lexecution
unit_tests_CPPFLAGS = -I$(abs_top_srcdir)/src
