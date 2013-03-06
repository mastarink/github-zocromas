" set syntax on
" win 155 53
" winpos 2 25
" :let mas_incdir = 'inc'

" sbuffer %
" e source.vim
set path=src,src/inc
" src,src/server,src/server/inc,src/inc,src/fileinfo,src/fileinfo/inc,src/io,src/io/inc,src/init,src/init/inc,src/cli,src/cli/inc,src/cmdmods,src/cmdmods/inc,src/control,src/control/inc,src/client,src/client/inc,src/http,src/http/inc,src/listener,src/listener/inc,src/channel,src/channel/inc,src/modules,src/modules/inc,src/transaction,src/transaction/inc,src/variables,src/variables/inc,src/log,src/log/inc,src/xcromas,src/xcromas/inc
find main.c

" Init

sfind mas_server_def.h


" tab sfind mas_opts.c
" sfind mas_opts.h
" sfind mas_opts_data.c

" tab sfind mas_init.c
" sfind mas_init.h
" sfind mas_sig.c
" sfind mas_sig.h

" tab sfind mas_cli_options.c
" sfind mas_cli_options.h



tab sfind mas_basic_def.h

tab sfind minitest.c
sfind parg.c
