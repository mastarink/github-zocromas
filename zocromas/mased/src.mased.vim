" set syntax on
" win 155 53
" winpos 2 25
" :let mas_incdir = 'inc'

" sbuffer %
" e source.vim
find main.c
sfind mas_server_main.c
sfind mas_client_main.c

tab sfind mas_transaction.c
sfind mas_transaction_control.c
sfind mas_transaction.h
sfind mas_transaction_control.h
sfind mas_rcontrol_object.c
sfind mas_rcontrol_object.h

" Init
tab sfind mas_init_client.c
sfind mas_init_server.c
sfind mas_init_threads.c
sfind mas_init_threads.h
sfind mas_init.c
sfind mas_init.h
sfind mas_cli_options.c
sfind mas_cli_options.h
sfind mas_opts.c
sfind mas_opts.h
sfind mas_opts_data.c


tab sfind mas_master.c
sfind mas_master.h

tab sfind mas_ticker.c
sfind mas_ticker.h

tab sfind mas_watcher.c
sfind mas_watcher.h

tab sfind mas_lcontrol_object.c
sfind mas_listener.c
sfind mas_listener_control.c
sfind mas_listener_control_list.c
sfind mas_listeners.c
sfind mas_listener_wait_client.c
sfind mas_lcontrol_object.h
sfind mas_listener.h
sfind mas_listener_control.h
sfind mas_listener_control_list.h
sfind mas_listeners.h
sfind mas_listener_wait_client.h




" Client
tab sfind mas_init_client.c
sfind mas_init_client.h
sfind mas_client.c
sfind mas_client_main.c
sfind mas_client_sig.c
sfind mas_client_session.c
sfind mas_client_readline.c
sfind mas_client_readline.h
sfind mas_client_session.h
sfind mas_client.h

tab sfind mas_server_tools.c
sfind mas_server_tools.h

tab sfind mas_sig.c
sfind mas_sig.h
sfind mas_client_sig.c
sfind mas_server_sig.c
sfind mas_server_sig.h

tab sfind mas_basic_def.h
sfind mas_client_def.h
sfind mas_server_def.h

tab sfind minitest.c
sfind parg.c
