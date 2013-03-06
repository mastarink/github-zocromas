SUBDIRS= inc
bin_PROGRAMS=zocromas_client

zocromas_client_SOURCES=  mas_client_main.c mas_client_sig.c mas_client_readline.c \
	mas_client.c mas_client_session.c mas_init_client.c

zocromas_client_CFLAGS=@AM_CFLAGS@  @DEPS_CFLAGS@ -DMAS_NO_THREADS
zocromas_client_LDFLAGS=@AM_LDFLAGS@   @LIBS@ @DEPS_LIBS@ @DEPSX_LIBS@ -rdynamic

