SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_mas_mulconfnt.la
libzocromas_mas_mulconfnt_la_SOURCES = \
	parse.c \
	source_defaults.c \
	source_list_base.c \
	source_list.c \
	source_base.c \
	source.c \
	option_base.c \
	option.c \
	mulconfnt_error.c

libzocromas_mas_mulconfnt_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@

if MAS_TEST
noinst_PROGRAMS=mastest

mastest_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c \
	mastest_0.c \
	mastest_1.c \
	mastest_1u.c \
	mastest_popt1.c \
	mastest_popt.c
else

endif
