SUBDIRS= inc
lib_LTLIBRARIES = libzocromas_mas_error.la


libzocromas_mas_error_la_SOURCES = mas_error_base.c mas_error_clear.c mas_error_names.c mas_error_regfind.c \
	mas_error_reporting.c mas_error_reporting_index.c
libzocromas_mas_error_la_CFLAGS=@AM_CFLAGS@ @DEPS_CFLAGS@
libzocromas_mas_error_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@


if MAS_TEST
noinst_PROGRAMS=merror_test

merror_test_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
merror_test_CFLAGS=@AM_CFLAGS@  -pthread

merror_test_SOURCES=merror_test.c
else

endif
