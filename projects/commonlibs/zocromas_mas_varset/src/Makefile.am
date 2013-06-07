SUBDIRS= inc

lib_LTLIBRARIES = libzocromas_mas_varset.la 

libzocromas_mas_varset_la_SOURCES = mas_varset_object.c mas_varset_search.c mas_varset_headtail.c \
	mas_varset.c mas_varset_vclass_object.c mas_varset_vclass.c mas_varset_vclass_search.c \
	mas_varset_vclass_element.c mas_varset_vclass_headtail.c mas_varset_vclass_namevalue.c
# libmas_tools_la_CFLAGS = ${AM_CFLAGS} -lmas_maslibother

mtest_LDFLAGS=@AM_LDFLAGS@  @DEPS_LIBS@ lib@PACKAGE_NAME@.la
mtest_CFLAGS=@AM_CFLAGS@  -pthread

bin_PROGRAMS=mtest
mtest_SOURCES=mas_varset_mtest.c
