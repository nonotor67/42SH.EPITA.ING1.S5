lib_LIBRARIES = libexecution.a

libexecution_a_SOURCES =  \
  execution.c \
  execution.h \
  exec_false.c \
  exec_true.c \
  builtins_bool.h \
  exec_echo.c \
  builtins_echo.h \
  builtins_cd.c \
  builtins_cd.h \
  exec_exit.c \
  builtins_exit.h \
  execution_conditions.c \
  execution_conditions.h \
  execution_commands.c \
  execution_commands.h \
  execution_redir.c \
  execution_redir.h \
  execution_negation.c \
  execution_negation.h \
  execution_or_and.c \
  execution_or_and.h \
  execution_pipeline.c \
  execution_pipeline.h \
  execution_loop.c \
  execution_loop.h \
  exec_dot.c \
  builtins_dot.h \
  builtins_export.c \
  builtins_export.h \
  execution_loop.h \
  exec_loop_commands.c \
  builtins_loop.h \
  execution_function.c \
  execution_function.h

libexecution_a_CPPFLAGS = -I$(top_srcdir)/src

libexecution_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libexecution.a