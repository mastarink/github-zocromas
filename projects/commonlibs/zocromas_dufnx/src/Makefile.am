SUBDIRS= inc

bin_PROGRAMS=zocromas_dufnx

zocromas_dufnx_SOURCES=\
	dufnx.c \
	dufnx_debug.c

#zocromas_dufnx_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ @DEPSN_CFLAGS@ @LIBCRYPTO_CFLAGS@   @LIBEXIF_CFLAGS@
#zocromas_dufnx_LDFLAGS=@AM_LDFLAGS@        @DEPS_LIBS@ @DEPSN_LIBS@     @LIBCRYPTO_LIBS@       @LIBEXIF_LIBS@    @LIBMAGIC_LIBS@ @ZLIB_LIBS@ @READLINE_LIBS@ -ldl
zocromas_dufnx_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@
zocromas_dufnx_LDFLAGS=@AM_LDFLAGS@        @DEPS_LIBS@


lib_LTLIBRARIES = libzocromas_dufnx.la
libzocromas_dufnx_la_SOURCES = \
	keep.c

libzocromas_dufnx_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@

if MAS_TEST
noinst_PROGRAMS=mastest

mastest_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c mastest_0.c mastest_fill.c mastest_fromfs.c mastest_fromdb.c \
	dufnx_debug.c
else

endif
