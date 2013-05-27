SUBDIRS= inc
lib_LTLIBRARIES = libzocromas_mas_autoobject.la

libzocromas_mas_autoobject_la_SOURCES     = mas_autoobject.c

mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@ libzocromas_mas_autoobject.la
mtest_CFLAGS=@AM_CFLAGS@  -pthread

bin_PROGRAMS=mtest
mtest_SOURCES=mtest.c
