SUBDIRS= inc

lib_LTLIBRARIES = libzocromas_mas_varvec.la 

libzocromas_mas_varvec_la_SOURCES = mas_varvec_object.c mas_varvec.c mas_varvec_search.c \
	mas_varvec_element.c mas_varvec_headtail.c mas_varvec_namevalue.c
# libmas_tools_la_CFLAGS = ${AM_CFLAGS} -lmas_maslibother

mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@
mtest_LDADD=lib@PACKAGE_NAME@.la
mtest_CFLAGS=@AM_CFLAGS@  -pthread

noinst_PROGRAMS=mtest
mtest_SOURCES=mas_varvec_mtest.c
