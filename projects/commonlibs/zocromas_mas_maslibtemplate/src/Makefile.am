SUBDIRS= inc
lib_LTLIBRARIES = libzocromas_mas_maslibtemplate.la 

libzocromas_mas_maslibtemplate_la_SOURCES = mas_maslibtemplate.c
# libmas_tools_la_CFLAGS = ${AM_CFLAGS} -lmas_maslibother

mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@ libzocromas_mas_maslibtemplate.la
mtest_CFLAGS=@AM_CFLAGS@  -pthread

bin_PROGRAMS=mtest
mtest_SOURCES=mas_maslibtemplate_mtest.c
