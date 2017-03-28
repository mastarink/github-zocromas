SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_mulconfnt.la
libzocromas_mulconfnt_la_SOURCES = \
	parse.c \
	tools.c \
	source_defaults.c \
	source_list_base.c \
	source_list.c \
	source_base.c \
	source.c \
	option_base.c \
	option.c \
	option_tablist_base.c \
	option_tablist.c
#	mulconfnt_error_base.c
#	mulconfnt_error.c

libzocromas_mulconfnt_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@
libzocromas_mulconfnt_la_CFLAGS=@AM_CFLAGS@ @DEPS_CFLAGS@

if MAS_TEST
noinst_PROGRAMS=mastest

mastest_LDFLAGS=@AM_LDFLAGS@ @DEPSTEST_LIBS@  lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@ @DEPSTEST_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c \
	mastest_0.c \
	mastest_1.c \
	mastest_1s.c \
	mastest_1mul.c \
	mastest_1u.c \
	mastest_1enf.c \
	mastest_2.c \
	mastest_2a.c \
	mastest_3.c \
	mastest_3a.c \
	mastest_3q.c \
	mastest_4.c \
	mastest_5.c \
	mastest_6.c \
	mastest_popt1.c \
	mastest_popt.c
else

endif
