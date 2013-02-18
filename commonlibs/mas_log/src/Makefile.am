SUBDIRS= inc
lib_LTLIBRARIES = libmas_log.la libmas_logc.la libmas_logs.la


libmas_logs_la_SOURCES          = mas_log.c mas_logger.c
libmas_logs_la_CFLAGS=@AM_CFLAGS@  -pthreads
libmas_logs_la_LDFLAGS=@AM_LDFLAGS@   @LIBS@

libmas_logc_la_SOURCES          = mas_log.c mas_logger.c
libmas_logc_la_CFLAGS=@AM_CFLAGS@  -DMAS_NO_THREADS
libmas_logc_la_LDFLAGS=@AM_LDFLAGS@

libmas_log_la_SOURCES          = mas_log.c mas_logger.c
libmas_log_la_LDFLAGS=@AM_LDFLAGS@   @LIBS@

# pkglib_LTLIBRARIES = mas_io.la
# mas_io_la_SOURCES = mas_cs.c mas_io.c mas_sig.c mas_open.c mas_listen.c
# mas_io_la_LDFLAGS = -module -avoid-version -no-undefined



