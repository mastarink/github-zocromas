set path=src/inc/
" set syntax on
" win 155 53
" winpos 2 25
" :let mas_incdir = 'inc'

" sbuffer %
" e source.vim

find mas_common_types.h
sfind mas_common_defs.h
sfind mas_common_cdefs.h

tab sfind mas_evaluate_types.h

tab sfind mas_base_output_types.h

tab sfind mas_unidata_types.h
sfind mas_fileinfo_types.h

tab sfind     mas_message_types.h
sfind mas_transaction_control_types.h
sfind mas_opts_types.h

tab sfind mas_autoset_types.h 
sfind mas_autoobject_types.h 

tab sfind mas_varset_types.h 
sfind mas_varvec_types.h 

tab sfind  mas_control_types.h

tab sfind mas_listener_control_types.h
sfind mas_variables_types.h

tab sfind mas_unimodule_types.h
sfind mas_modules_types.h

sfind mas_thread_variables_types.h
sfind mas_th_types.h


tab sfind mas_channel_types.h 

