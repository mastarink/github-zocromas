SUBDIRS= inc

bin_PROGRAMS=mas_server

mas_server_SOURCES= mas_server_main.c mas_server_sig.c  \
	mas_master.c mas_server_tools.c mas_init_threads.c \
	mas_init_server.c mas_ticker.c mas_watcher.c
mas_server_CFLAGS=${AM_CFLAGS} -pthread \
	-luuid \
	-lmas_wrap -lmas_tools -lmas_arg_tools -lmas_io -lmas_channel \
	-L../control     -lmas_control \
	-L../log	 -lmas_log \
	-L../zoctools    -lmas_zoctools -lmas_msgtools -lmas_thread_tools \
	-L../variables   -lmas_variables \
	-L../init        -lmas_init \
	-L../listener    -lmas_listener \
	-L../transaction -lmas_transaction \
	-L../http        -lmas_http \
	-L../fileinfo    -lmas_fileinfo \
	-L../modules     -lmas_modules \
	-L../xcromas     -lmas_xcromas \
	-L../xcromas     -lmas_xcromasio \
	-rdynamic



