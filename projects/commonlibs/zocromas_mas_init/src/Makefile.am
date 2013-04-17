SUBDIRS= inc
lib_LTLIBRARIES = libzocromas_mas_init.la 

libzocromas_mas_init_la_SOURCES = mas_cli_options.c mas_init.c mas_opts.c mas_opts_storage.c mas_opts_save.c mas_opts_restore.c \
	mas_opts_common.c mas_opts_data.c mas_sig.c
libzocromas_mas_init_la_CFLAGS = @AM_CFLAGS@ -DMAS_NO_THREADS
libzocromas_mas_init_la_LDFLAGS = @AM_LDFLAGS@ @DEPS_LIBS@
# libmas_tools_la_CFLAGS = ${AM_CFLAGS} -lmas_maslibother

