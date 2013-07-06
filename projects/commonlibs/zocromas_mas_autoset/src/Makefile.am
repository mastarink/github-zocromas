SUBDIRS= inc

lib_LTLIBRARIES = libzocromas_mas_autoset.la 

libzocromas_mas_autoset_la_SOURCES = mas_autoset_object.c mas_autoset_search.c mas_autoset.c
# libmas_tools_la_CFLAGS = ${AM_CFLAGS} -lmas_maslibother
libzocromas_mas_autoset_la_LDFLAGS=@AM_LDFLAGS@   @DEPS_LIBS@


mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@ lib@PACKAGE_NAME@.la -lrt
mtest_CFLAGS=@AM_CFLAGS@  -pthread

bin_PROGRAMS=mtest
mtest_SOURCES=mas_autoset_mtest.c
