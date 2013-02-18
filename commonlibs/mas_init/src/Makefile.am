SUBDIRS= inc
lib_LTLIBRARIES = libmas_init.la 

libmas_init_la_SOURCES = mas_cli_options.c  mas_init.c  mas_opts.c  mas_opts_data.c  mas_sig.c
libmas_init_la_CFLAGS = @AM_CFLAGS@ -DMAS_NO_THREADS
libmas_init_la_LDFLAGS = @AM_LDFLAGS@ @DEPSL_LIBS@
# libmas_tools_la_CFLAGS = ${AM_CFLAGS} -lmas_maslibother

