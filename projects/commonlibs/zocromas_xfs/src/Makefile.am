SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_xfs.la
libzocromas_xfs_la_SOURCES = \
	masxfs_scan.c \
	masxfs_pathinfo_base.c \
	masxfs_pathinfo.c \
	tools.c

libzocromas_xfs_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@
libzocromas_xfs_la_CFLAGS=@AM_CFLAGS@ @DEPS_CFLAGS@

if MAS_TEST
noinst_PROGRAMS=mastest mastest1

mastest_LDFLAGS=@AM_LDFLAGS@ @DEPSTEST_LIBS@  lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@ @DEPSTEST_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c \
	masxfs_test0.c

mastest1_LDFLAGS=@AM_LDFLAGS@ @DEPSTEST_LIBS@  lib@PACKAGE_NAME@.la
mastest1_CFLAGS=@AM_CFLAGS@ @DEPSTEST_CFLAGS@  -pthread

mastest1_SOURCES= \
	mastest1.c
else

endif
