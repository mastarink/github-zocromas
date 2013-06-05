SUBDIRS= inc
instdir=mastar
lib_LTLIBRARIES = \
	libzocromas_mas_arg_tools.la \
	libzocromas_mas_tools.la

libzocromas_mas_arg_tools_la_SOURCES    = mas_arg_tools.c mas_argx_tools.c
libzocromas_mas_tools_la_SOURCES        = mas_tools.c
# libzocromas_mas_tools_la_CPPFLAGS = @AM_CPPFLAGS@ @DEPS_CPPFLAGS@
# libzocromas_mas_tools_la_CFLAGS = @AM_CFLAGS@ -lmas_thread_tools

mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@ lib@PACKAGE_NAME@.la libzocromas_mas_arg_tools.la
mtest_CFLAGS=@AM_CFLAGS@  -pthread

bin_PROGRAMS=mtest
mtest_SOURCES=mas_tools_mtest.c
