SUBDIRS= inc
lib_LTLIBRARIES = libzocromas_mas_error.la


libzocromas_mas_error_la_SOURCES = mas_template.c
libzocromas_mas_error_la_LDFLAGS=@AM_LDFLAGS@


if MAS_TEST
noinst_PROGRAMS=merror_test

merror_test_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
merror_test_CFLAGS=@AM_CFLAGS@  -pthread

merror_test_SOURCES=merror_test.c
else

endif
