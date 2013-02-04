SUBDIRS=inc
bin_PROGRAMS=mas_client

mas_client_SOURCES=  mas_client_main.c mas_client_sig.c mas_client_readline.c \
	mas_client.c mas_client_session.c mas_init_client.c

mas_client_CFLAGS=${AM_CFLAGS} \
	-lreadline       -luuid \
	-lmas_wrap -lmas_tools -lmas_arg_tools \
	-L../channel     -lmas_channel \
	-L../xcromas     -lmas_xcromasio \
	-L../control     -lmas_control \
	-L../log	 -lmas_log \
	-L../io          -lmas_io \
	-L../zoctools    -lmas_zoctools -lmas_msgtools -lmas_thread_tools \
	-L../variables   -lmas_variables \
	-L../init        -lmas_init \
	-rdynamic
