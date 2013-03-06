" echo 'in ' . expand('<sfile>')

source gvim-funcs.vim

set swb=usetab,newtab,split
set undolevels=10000
set virtualedit=block

tabfirst
execute "sbuffer " . expand('%')
" call append(0, '/* WOW of ' . expand('%:t') . ' ' . strftime("%c") . ' */')
bw
tabmove
tabmove 0
nmap <A-Up> :sbprev<Enter><C-W>_
nmap <A-Down> :sbnext<Enter><C-W>_

nmap <C-\> <ESC>:execute "sbuffer " . expand('<cWORD>')<CR>


" winpos 0 35
" winsize 155 50



" sb `=dfile`
" bd `=dfile`
" echo 'out ' . expand('<sfile>')
