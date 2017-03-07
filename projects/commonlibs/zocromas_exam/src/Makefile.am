SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_exam.la
libzocromas_exam_la_SOURCES = \
 	masexam.c

libzocromas_exam_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@
libzocromas_exam_la_CFLAGS=@AM_CFLAGS@ @DEPS_CFLAGS@

if MAS_TEST
noinst_PROGRAMS=mastest

# mastest_LDFLAGS=@AM_LDFLAGS@ @DEPSTEST_LIBS@  lib@PACKAGE_NAME@.la
# mastest_CFLAGS=@AM_CFLAGS@ @DEPSTEST_CFLAGS@  -pthread
mastest_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c
else

endif
