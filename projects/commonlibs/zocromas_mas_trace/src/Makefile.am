SUBDIRS= inc
lib_LTLIBRARIES = libzocromas_mas_trace.la


libzocromas_mas_trace_la_SOURCES = mas_trace.c mas_trace_utils_print.c
libzocromas_mas_trace_la_LDFLAGS=@AM_LDFLAGS@


if MAS_TEST
noinst_PROGRAMS=mtrace_test

mtrace_test_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
# mtest_LDADD=libzocromas_mas_arg_tools.la
mtrace_test_CFLAGS=@AM_CFLAGS@  -pthread

mtrace_test_SOURCES=mtrace_test.c
else

endif
