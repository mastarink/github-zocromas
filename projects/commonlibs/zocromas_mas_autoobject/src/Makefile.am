SUBDIRS= inc
lib_LTLIBRARIES = libzocromas_mas_autoobject.la

libzocromas_mas_autoobject_la_SOURCES     = mas_autoobject_object.c  mas_autoobject.c
libzocromas_mas_autoobject_la_LDFLAGS = @AM_LDFLAGS@   @LIBS@ @DEPS_LIBS@

mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@ -lrt
mtest_LDADD=lib@PACKAGE_NAME@.la
mtest_CFLAGS=@AM_CFLAGS@  -pthread

noinst_PROGRAMS=mtest
mtest_SOURCES=mas_autoobject_mtest.c
