SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_mas_mulconfnt.la
libzocromas_mas_mulconfnt_la_SOURCES = parse.c source_list.c source.c option.c

libzocromas_mas_mulconfnt_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@

if MAS_TEST
noinst_PROGRAMS=mastest

mastest_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@  -pthread

mastest_SOURCES=mastest.c
else

endif
