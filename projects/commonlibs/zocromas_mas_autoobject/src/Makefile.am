SUBDIRS= inc
lib_LTLIBRARIES = libzocromas_mas_autoobject.la

libzocromas_mas_autoobject_la_SOURCES     = mas_autoobject_object.c  mas_autoobject.c

mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@ lib@PACKAGE_NAME@.la -lrt
mtest_CFLAGS=@AM_CFLAGS@  -pthread

bin_PROGRAMS=mtest
mtest_SOURCES=mas_autoobject_mtest.c
