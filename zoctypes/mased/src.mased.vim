set path=inc/,src/inc/
" set syntax on
" win 155 53
" winpos 2 25
" :let mas_incdir = 'inc'

" sbuffer %
" e source.vim

find mas_common_types.h

tab sfind mas_http_types.h
sfind mas_unidata_types.h
sfind mas_fileinfo_types.h

tab sfind     mas_message_types.h
sfind mas_transaction_control_types.h
sfind mas_opts_types.h

tab sfind  mas_control_types.h

tab sfind mas_listener_control_types.h
sfind mas_variables_types.h
sfind mas_modules_types.h
sfind mas_thread_variables_types.h
sfind mas_th_types.h
