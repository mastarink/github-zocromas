SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_regerr.la
libzocromas_regerr_la_SOURCES = \
 	masregerr.c \
 	masregerr_base.c

libzocromas_regerr_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@

if MAS_TEST
noinst_PROGRAMS=mastest

mastest_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c
else

endif
