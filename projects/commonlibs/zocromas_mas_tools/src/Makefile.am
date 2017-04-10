SUBDIRS= inc
instdir=mastar
lib_LTLIBRARIES = libzocromas_mas_tools.la

#libzocromas_mas_arg_tools_la_SOURCES    = mas_arg_tools.c mas_argvc_tools.c mas_argx_tools.c mas_convert.c mas_time.c mas_expandable.c mas_utils_path.c
#libzocromas_mas_arg_tools_la_LDFLAGS = @AM_LDFLAGS@ @DEPS_LIBS@

libzocromas_mas_tools_la_SOURCES        = mas_tools.c mas_arg_tools.c mas_argvc_tools.c mas_argx_tools.c mas_convert.c mas_time.c mas_expandable.c mas_utils_path.c
libzocromas_mas_tools_la_CFLAGS=@AM_CFLAGS@ @DEPS_CFLAGS@
libzocromas_mas_tools_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@

if MAS_TEST
noinst_PROGRAMS=mtest

mtest_LDFLAGS=@AM_LDFLAGS@
mtest_LDADD=lib@PACKAGE_NAME@.la
# mtest_LDADD=libzocromas_mas_arg_tools.la
mtest_CFLAGS=@AM_CFLAGS@  -pthread

mtest_SOURCES=mas_tools_mtest.c
else

endif
