SUBDIRS= inc
lib_LTLIBRARIES = libzocromas_mas_log.la libzocromas_mas_logc.la libzocromas_mas_logs.la


libzocromas_mas_logs_la_SOURCES          = mas_log.c mas_logger.c
libzocromas_mas_logs_la_CFLAGS=@AM_CFLAGS@  -pthread
libzocromas_mas_logs_la_LDFLAGS=@AM_LDFLAGS@   @LIBS@

libzocromas_mas_logc_la_SOURCES          = mas_log.c mas_logger.c
libzocromas_mas_logc_la_CFLAGS=@AM_CFLAGS@  -DMAS_NO_THREADS
libzocromas_mas_logc_la_LDFLAGS=@AM_LDFLAGS@

libzocromas_mas_log_la_SOURCES          = mas_log.c mas_logger.c
libzocromas_mas_log_la_LDFLAGS=@AM_LDFLAGS@   @LIBS@

# pkglib_LTLIBRARIES = mas_io.la
# mas_io_la_SOURCES = mas_cs.c mas_io.c mas_sig.c mas_open.c mas_listen.c
# mas_io_la_LDFLAGS = -module -avoid-version -no-undefined



