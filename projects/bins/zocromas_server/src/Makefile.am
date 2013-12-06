SUBDIRS= inc
bin_PROGRAMS=zocromas_server

zocromas_server_SOURCES= mas_server_main.c mas_server_sig.c mas_server_exit.c  \
	mas_master.c \
	mas_master_core.c \
	mas_master_thread.c \
	mas_bunch.c mas_server_tools.c \
	mas_init_threads.c mas_init_protos.c mas_init_pid.c mas_init_daemon.c \
	mas_init_server.c mas_ticker.c mas_watcher.c
zocromas_server_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@
zocromas_server_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ @DEPSX_LIBS@ -rdynamic
