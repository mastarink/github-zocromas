SUBDIRS= inc

lib_LTLIBRARIES = libzocromas_mas_varset.la 

libzocromas_mas_varset_la_SOURCES = mas_varset.c
# libmas_tools_la_CFLAGS = ${AM_CFLAGS} -lmas_maslibother

mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@
mtest_CFLAGS=@AM_CFLAGS@  -pthread

bin_PROGRAMS=mtest
mtest_SOURCES=mas_varset_mtest.c
