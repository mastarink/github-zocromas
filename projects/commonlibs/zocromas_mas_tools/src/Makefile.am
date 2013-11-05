SUBDIRS= inc
instdir=mastar
lib_LTLIBRARIES = libzocromas_mas_arg_tools.la libzocromas_mas_tools.la

libzocromas_mas_arg_tools_la_SOURCES    = mas_arg_tools.c mas_argx_tools.c
libzocromas_mas_arg_tools_la_LDFLAGS = @AM_LDFLAGS@ @DEPS_LIBS@

libzocromas_mas_tools_la_SOURCES        = mas_tools.c
# libzocromas_mas_tools_la_CPPFLAGS = @AM_CPPFLAGS@ @DEPS_CPPFLAGS@
libzocromas_mas_tools_la_LDFLAGS = @AM_LDFLAGS@ @DEPS_LIBS@

if MAS_TEST
noinst_PROGRAMS=mtest

mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@ lib@PACKAGE_NAME@.la
mtest_LDADD=libzocromas_mas_arg_tools.la
mtest_CFLAGS=@AM_CFLAGS@  -pthread

mtest_SOURCES=mas_tools_mtest.c
else

endif
