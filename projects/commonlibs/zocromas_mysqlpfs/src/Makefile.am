SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_mysqlpfs.la
libzocromas_mysqlpfs_la_SOURCES = \
 	mysqlpfs.c

libzocromas_mysqlpfs_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ @MYSQL_CLIENT_LIBS@
libzocromas_mysqlpfs_la_CFLAGS=@AM_CFLAGS@ @DEPS_CFLAGS@ @MYSQL_CLIENT_CFLAGS@

if MAS_TEST
noinst_PROGRAMS=mastest0 mastest1 mastest mastestb

mastest0_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la @MYSQL_CLIENT_LIBS@
mastest0_CFLAGS=@AM_CFLAGS@  @MYSQL_CLIENT_CFLAGS@  -pthread

mastest0_SOURCES= \
	mastest0.c

mastest1_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la @MYSQL_CLIENT_LIBS@
mastest1_CFLAGS=@AM_CFLAGS@  @MYSQL_CLIENT_CFLAGS@  -pthread

mastest1_SOURCES= \
	mastest1.c



mastest_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la @MYSQL_CLIENT_LIBS@
mastest_CFLAGS=@AM_CFLAGS@  @MYSQL_CLIENT_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c

mastestb_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la @MYSQL_CLIENT_LIBS@
mastestb_CFLAGS=@AM_CFLAGS@  @MYSQL_CLIENT_CFLAGS@  -pthread

mastestb_SOURCES= \
	mastestb.c




else

endif
