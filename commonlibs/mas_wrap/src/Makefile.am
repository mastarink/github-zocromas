SUBDIRS= inc
# ?? libmas_wraplibdir = $(libdir)/mastar/wrap
lib_LTLIBRARIES = libmas_wraps.la libmas_wrapc.la libmas_wrap.la

#? libmas_wraaplibdir = $(libdir)/mastar
libmas_wraps_la_SOURCES         = mas_lib.c mas_lib0.c mas_memory.c mas_lib_thread.c
libmas_wraps_la_CFLAGS=@AM_CFLAGS@   -pthreads
libmas_wraps_la_LDFLAGS=@AM_LDFLAGS@   @LIBS@

libmas_wrapc_la_SOURCES         = mas_lib.c mas_lib0.c mas_memory.c
libmas_wrapc_la_CFLAGS=@AM_CFLAGS@  -DMAS_NO_THREADS
libmas_wrapc_la_LDFLAGS=@AM_LDFLAGS@

libmas_wrap_la_SOURCES         = mas_lib.c mas_lib0.c mas_memory.c mas_lib_thread.c
libmas_wrap_la_LDFLAGS=@AM_LDFLAGS@   @LIBS@
