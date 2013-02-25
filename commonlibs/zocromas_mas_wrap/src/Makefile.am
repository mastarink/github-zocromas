SUBDIRS= inc
# ?? libzocromas_mas_wraplibdir = $(libdir)/mastar/wrap
lib_LTLIBRARIES = libzocromas_mas_wraps.la libzocromas_mas_wrapc.la libzocromas_mas_wrap.la

#? libmas_wraaplibdir = $(libdir)/mastar
libzocromas_mas_wraps_la_SOURCES         = mas_lib.c mas_lib0.c mas_memory.c mas_lib_thread.c
libzocromas_mas_wraps_la_CFLAGS=@AM_CFLAGS@   -pthread
libzocromas_mas_wraps_la_LDFLAGS=@AM_LDFLAGS@   @LIBS@

libzocromas_mas_wrapc_la_SOURCES         = mas_lib.c mas_lib0.c mas_memory.c
libzocromas_mas_wrapc_la_CFLAGS=@AM_CFLAGS@  -DMAS_NO_THREADS
libzocromas_mas_wrapc_la_LDFLAGS=@AM_LDFLAGS@

libzocromas_mas_wrap_la_SOURCES         = mas_lib.c mas_lib0.c mas_memory.c mas_lib_thread.c
libzocromas_mas_wrap_la_LDFLAGS=@AM_LDFLAGS@   @LIBS@
