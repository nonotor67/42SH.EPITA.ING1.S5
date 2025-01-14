lib_LIBRARIES = libexecution.a

libexecution_a_SOURCES =  \
  execution.c \
  execution.h \
  exec_false.c \
  exec_true.c \
  builtins_bool.h \
  exec_echo.c \
  builtins_echo.h \
  exec_exit.c \
  builtins_exit.h \
  execution_conditions.c \
  execution_conditions.h \
  execution_commands.c \
  execution_commands.h \
  execution_redir.c \
  execution_redir.h \
  execution_pipeline.c \
  execution_pipeline.h

libexecution_a_CPPFLAGS = -I$(top_srcdir)/src

libexecution_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libexecution.a