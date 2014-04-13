" echo 'in ' . expand('<sfile>')

source gvim-funcs.vim

set swb=usetab,newtab,split
set undolevels=10000
set virtualedit=block

" if exists(dfile) && !empty(dfile)
" tabfirst
"   execute "sbuffer " . dfile
" " call setline(12, "GGG")
" " call append(0, '/* WOW of ' . expand('%:t') . ' ' . strftime("%c") . ' */')
"   bw
"   tabmove
"   tabmove 0

" " ??????????
"   nmap <A-Up> :sbprev<Enter><C-W>_
"   nmap <A-Down> :sbnext<Enter><C-W>_

"   nmap <C-\> <ESC>:execute "sbuffer " . expand('<cWORD>')<CR>
" endif


let shn_doprj="shn/doprj.sh"
if filereadable(shn_doprj)
" echo "has doprj:" . shn_doprj
  map <F3>   :wa<CR>:!shn/doprj.sh i<CR>
  map <C-F3> :wa<CR>:!shn/doprj.sh c i<CR>
endif
let shn_doall="shn/doall.sh"
if filereadable(shn_doall)
" echo "has doall:" . shn_doall
  map <F4>   :wa<CR>:!shn/doall.sh i<CR>
  map <C-F4> :wa<CR>:!shn/doall.sh c i<CR>
endif


" winpos 0 35
" winsize 155 50



" sb `=dfile`
" bd `=dfile`
" echo 'out ' . expand('<sfile>')
