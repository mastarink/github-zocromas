SUBDIRS= inc
bin_PROGRAMS=mas_server

mas_server_SOURCES= mas_server_main.c mas_server_sig.c  \
	mas_master.c mas_server_tools.c mas_init_threads.c \
	mas_init_server.c mas_ticker.c mas_watcher.c
mas_server_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@
mas_server_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ @DEPSX_LIBS@ -rdynamic
