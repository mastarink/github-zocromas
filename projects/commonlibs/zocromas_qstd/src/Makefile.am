SUBDIRS= inc


lib_LTLIBRARIES = libzocromas_qstd.la
libzocromas_qstd_la_SOURCES = \
	qstd_mstmt_base.c \
	qstd_mstmt.c \
	qstd_mstmt_names.c \
	qstd_mstmt_sizes.c \
	qstd_mstmt_datas.c \
	qstd_mstmt_props.c \
	qstd_mstmt_parents.c \
	qstd_mstmt_nodes.c

libzocromas_qstd_la_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ @MYSQL_CLIENT_LIBS@
libzocromas_qstd_la_CFLAGS=@AM_CFLAGS@ @DEPS_CFLAGS@ @MYSQL_CLIENT_CFLAGS@ -fvisibility=hidden

if MAS_TEST
noinst_PROGRAMS=mastest



mastest_LDFLAGS=@AM_LDFLAGS@  lib@PACKAGE_NAME@.la @DEPSTEST_LIBS@ @MYSQL_CLIENT_LIBS@
mastest_CFLAGS=@AM_CFLAGS@  @DEPSTEST_CFLAGS@ @MYSQL_CLIENT_CFLAGS@  -pthread

mastest_SOURCES= \
	mastest.c \
	mastest_connect.c \
	mastest_drop_tables.c \
	mastest_create_tables.c




else

endif
