SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_mysqlpfs.la
libzocromas_mysqlpfs_la_SOURCES = \
 	mysqlpfs.c \
 	mysqlpfs_query.c \
	mysqlpfs_base.c \
	mysqlpfs_mstmt_base.c \
	mysqlpfs_mstmt.c \
	mysqlpfs_mstmt_d.c

libzocromas_mysqlpfs_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ @MYSQL_CLIENT_LIBS@
libzocromas_mysqlpfs_la_CFLAGS=@AM_CFLAGS@ @DEPS_CFLAGS@ @MYSQL_CLIENT_CFLAGS@

if MAS_TEST
noinst_PROGRAMS=mastest0 mastest1 mastest mastestb mastestbind

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
	mastest.c \
	mastest_connect.c \
	mastest_1.c \
	mastest_2.c \
	mastest_3a.c \
	mastest_3b.c \
	mastest_4.c \
	mastest_4o.c \
	mastest_5.c



mastestb_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la @MYSQL_CLIENT_LIBS@
mastestb_CFLAGS=@AM_CFLAGS@  @MYSQL_CLIENT_CFLAGS@  -pthread

mastestb_SOURCES= \
	mastestb.c

mastestbind_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la @MYSQL_CLIENT_LIBS@
mastestbind_CFLAGS=@AM_CFLAGS@  @MYSQL_CLIENT_CFLAGS@  -pthread

mastestbind_SOURCES= \
	mastestbind.c





else

endif
