SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_exam.la
libzocromas_exam_la_SOURCES = \
 	masexam.c

libzocromas_exam_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@

if MAS_TEST
noinst_PROGRAMS=mastest

mastest_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c
else

endif
