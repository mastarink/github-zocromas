SUBDIRS= inc

lib_LTLIBRARIES = libzocromas_mas_channel.la libzocromas_mas_channel_nooc.la

libzocromas_mas_channel_la_SOURCES      = mas_channel.c mas_channel_object.c mas_channel_open.c mas_channel_listen.c mas_channel_socket.c
libzocromas_mas_channel_la_LDFLAGS      =  @AM_LDFLAGS@ @DEPSL_LIBS@

libzocromas_mas_channel_nooc_la_SOURCES      = mas_channel.c mas_channel_object.c mas_channel_open.c mas_channel_listen.c mas_channel_socket.c
libzocromas_mas_channel_nooc_la_CFLAGS      =  @AM_CFLAGS@ -DMAS_NO_CTRL -DMAS_NO_OPTS
libzocromas_mas_channel_nooc_la_LDFLAGS      =  @AM_LDFLAGS@ @DEPSNOOCL_LIBS@
