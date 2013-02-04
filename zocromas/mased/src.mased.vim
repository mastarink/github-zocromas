" set syntax on
" win 155 53
" winpos 2 25
" :let mas_incdir = 'inc'
" :set path=src,src/server,src/server/inc,src/inc,src/fileinfo,src/fileinfo/inc,src/cmdmods,src/cmdmods/inc,src/maslibs,src/maslibs/inc,src/client,src/client/inc,src/channel,src/channel/inc
" set path=src,src/server,src/server/inc,src/inc,src/fileinfo,src/fileinfo/inc,src/io,src/io/inc,src/init,src/init/inc,src/zoctools,src/zoctools/inc,src/cmdmods,src/cmdmods/inc,src/control,src/control/inc,src/client,src/client/inc,src/http,src/http/inc,src/listener,src/listener/inc,src/channel,src/channel/inc,src/modules,src/modules/inc,src/transaction,src/transaction/inc,src/variables,src/variables/inc,src/log,src/log/inc,src/xcromas,src/xcromas/inc

" sbuffer %
" e source.vim
find main.c
sfind mas_server_main.c
sfind mas_client_main.c

tab sfind mas_http_get.c
sfind mas_http_request.c
sfind mas_http_request.h
sfind mas_http_reply.c
sfind mas_http_reply.h
sfind mas_http_utils.c
sfind mas_http_utils.h
sfind mas_http_types.h
sfind mas_transaction_http.c
sfind mas_transaction_http.h
sfind mas_http_get.h

" finish

tab sfind mas_fileinfo.c
sfind mas_fileinfo_object.c
sfind mas_unidata.c
sfind mas_fileinfo.h
sfind mas_unidata.h
sfind mas_unidata_types.h
sfind mas_fileinfo_object.h
sfind mas_fileinfo_types.h

tab sfind     mas_message_types.h
sfind     mas_transaction_xcromas.c
sfind     mas_transaction_xcromas.h
sfind mas_message_io.c
sfind mas_message_io.h

tab sfind mas_transaction.c
sfind mas_transaction_control.c
sfind mas_transaction.h
sfind mas_transaction_control.h
sfind mas_transaction_control_types.h
sfind mas_rcontrol_object.c
sfind mas_rcontrol_object.h

tab sfind mas_cmdmod_client.c
sfind mas_cmdmod_get.c
sfind mas_cmdmod_mem.c
sfind mas_cmdmod_check.c
sfind mas_cmdmod_readline.c
sfind mas_cmdmod_server.c
sfind mas_cmdmod_listener.c
sfind mas_cmdmod_set.c
sfind mas_cmdmod_ctrl.c
sfind mas_cmdmod_show.c
sfind mas_cmdmod_do.c
sfind mas_cmdmod_root.c

" Handling 'command' modules
tab sfind mas_modules_commands.c
sfind mas_modules_commands_eval.c
sfind mas_modules_commands_eval.h
sfind mas_modules_commands.h
sfind mas_modules_load_module.c
sfind mas_modules_load_module.h
sfind mas_modules_types.h

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
sfind mas_opts_types.h


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
sfind mas_listener_control_types.h
sfind mas_listeners.h
sfind mas_listener_wait_client.h






tab sfind mas_thread_variables.c
sfind mas_thread_variables.h
sfind mas_variables.c
sfind mas_variables.h
sfind mas_variables_types.h
sfind mas_thread_variables_types.h
sfind mas_th_types.h


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

tab sfind mas_channel.c
sfind mas_channel_object.c
sfind mas_channel_object.h
sfind mas_channel_listen.c
sfind mas_channel_open.c
sfind mas_channel_socket.c
sfind mas_channel.h
sfind mas_channel_listen.h
sfind mas_channel_open.h
sfind mas_channel_socket.h
sfind mas_channel_types.h

tab sfind mas_io.c
sfind mas_io.h

tab sfind mas_log.c
sfind mas_logger.c
sfind mas_log.h
sfind mas_logger.h
sfind mas_log_types.h

tab sfind mas_msg_tools.c
sfind mas_tools_def.h
sfind mas_msg_tools.h
sfind mas_msg_def.h

tab sfind mas_server_tools.c
sfind mas_server_tools.h

tab sfind mas_thread_tools.c
sfind mas_thread_tools.h

tab sfind mas_curses.c
sfind mas_curses.h

tab sfind mas_sig.c
sfind mas_sig.h
sfind mas_client_sig.c
sfind mas_server_sig.c
sfind mas_server_sig.h

tab sfind mas_control.c
sfind mas_control_data.c
sfind mas_control.h
sfind mas_control_types.h
sfind mas_global.h

tab sfind mas_lib_thread.c
sfind mas_lib_thread.h

tab sfind mas_basic_def.h
sfind mas_tools_def.h
sfind mas_list_def.h

tab sfind mas_common.h
sfind mas_common_types.h

tab sfind minitest.c
sfind parg.c
