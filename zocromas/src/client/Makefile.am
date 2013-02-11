SUBDIRS= inc
bin_PROGRAMS=mas_client

mas_client_SOURCES=  mas_client_main.c mas_client_sig.c mas_client_readline.c \
	mas_client.c mas_client_session.c mas_init_client.c

mas_client_CFLAGS=${AM_CFLAGS} \
	-lreadline       -luuid \
	-L\$(prefix)/lib -lmas_wrap -lmas_tools -lmas_arg_tools -lmas_io -lmas_channel -lmas_variables -lmas_log -lmas_msg -lmas_message_io -lmas_thtools \
	-L../control     -lmas_control \
	-L../cli	 -lmas_cli \
	-L../init        -lmas_init \
	-rdynamic

