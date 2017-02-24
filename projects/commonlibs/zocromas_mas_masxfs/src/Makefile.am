SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_mas_masxfs.la
libzocromas_mas_masxfs_la_SOURCES = \
	masxfs_scan.c \
	masxfs_pathinfo_base.c \
	masxfs_pathinfo.c \
	masxfs_pathinfo_io.c \
	masxfs_levinfo_base.c \
	masxfs_levinfo_io.c \
	masxfs_levinfo_path.c \
	masxfs_levinfo_ref.c \
	masxfs_levinfo.c \
	tools.c

libzocromas_mas_masxfs_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@

if MAS_TEST
noinst_PROGRAMS=mastest mastest1

mastest_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
mastest_CFLAGS=@AM_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c \
	masxfs_test0.c

mastest1_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la
mastest1_CFLAGS=@AM_CFLAGS@  -pthread

mastest1_SOURCES= \
	mastest1.c
else

endif
