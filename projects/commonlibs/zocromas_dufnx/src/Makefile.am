SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_dufnx.la
libzocromas_dufnx_la_SOURCES = \
 	dufnx.c

libzocromas_dufnx_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@

if MAS_TEST
noinst_PROGRAMS=mastest

mastest_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c mastest_0.c mastest_1.c mastest_2.c mastest_3.c
else

endif
