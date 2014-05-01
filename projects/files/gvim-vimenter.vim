" echo 'in ' . expand('<sfile>')

let  gvim_funcs=mas_localvimdirs . 'gvim-funcs.vim'
if filereadable(gvim_funcs)
  source `=gvim_funcs`
else
  echo 'Not found: ' . gvim_funcs
endif

set swb=usetab,newtab,split
set undolevels=10000
set virtualedit=block

if exists('v:servername')
  let vrbprefix=mas_localvimdirs . '.gvimvrb/'
  if !isdirectory(vrbprefix)
    call mkdir(vrbprefix)
  endif
  if !isdirectory(vrbprefix)
    let vrbprefix=''
  endif
  let vrbfile=vrbprefix . v:servername . ".vrb"
  let redirfile=vrbprefix . v:servername . ".redir"
  let viminfofile=vrbprefix . v:servername . ".viminfo"
  execute 'set verbosefile=' . vrbfile
  execute 'redir >> ' . redirfile
  execute "set viminfo='50,<1000,s1000,n" . viminfofile
endif

if exists('g:maseddrop')
  execute 'tab drop ' . g:maseddrop
endif


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

nmap <A-1> :tabfirst<CR>
nmap <A-2> :tablast<CR>
let shn_doprj="shn/doprj.sh"
if filereadable(shn_doprj)
" echo "has doprj:" . shn_doprj
  nmap <F3>   :wa<CR>:!shn/doprj.sh i<CR>
  nmap <C-F3> :wa<CR>:!shn/doprj.sh c i<CR>
endif
let shn_doall="shn/doall.sh"
if filereadable(shn_doall)
" echo "has doall:" . shn_doall
  nmap <F4>   :wa<CR>:!shn/doall.sh i<CR>
  nmap <C-F4> :wa<CR>:!shn/doall.sh c i<CR>
endif
 
" insert filename
inoremap <F5> <C-R>=expand("%:t")<CR>

nmap <C-]> :call MasGoTag()<CR>


" winpos 0 35
" winsize 155 50



" sb `=dfile`
" bd `=dfile`
" echo 'out ' . expand('<sfile>')
