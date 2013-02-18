SUBDIRS= inc
bin_PROGRAMS=mas_client

mas_client_SOURCES=  mas_client_main.c mas_client_sig.c mas_client_readline.c \
	mas_client.c mas_client_session.c mas_init_client.c

mas_client_CFLAGS=${AM_CFLAGS}  ${DEPS_CFLAGS}
mas_client_LDFLAGS=${AM_LDFLAGS} -lreadline       -luuid \
	${DEPS_LIBS} ${DEPSX_LIBS} \
	-rdynamic

