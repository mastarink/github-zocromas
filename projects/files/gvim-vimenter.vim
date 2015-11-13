" echo 'in ' . expand('<sfile>')

set makeprg=make\ -C\ $MSH_SHN_BUILD_DIR
if isdirectory(mas_localvimdirs)
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
    let mas_gvimworkbase=mas_localvimdirs . '.gvim/'
    let mas_gvimwork=mas_gvimworkbase . strftime( "%Y%m%d" ) . '/'
    let vrbprefix=mas_gvimwork . 'vrb/'
    let redirprefix=mas_gvimwork . 'vrb/'
    let infoprefix=mas_gvimwork . 'info/'
    if exists( "*mkdir" )
      if ( !isdirectory(mas_gvimworkbase) )
	call mkdir( mas_gvimworkbase )
      endif
      if ( !isdirectory(mas_gvimwork) )
	call mkdir( mas_gvimwork )
      endif
      if ( !isdirectory(vrbprefix) )
	call mkdir( vrbprefix )
      endif
      if ( !isdirectory(redirprefix) )
	call mkdir( redirprefix )
      endif
      if ( !isdirectory(infoprefix) )
	call mkdir( infoprefix )
      endif
    endif
    if !isdirectory(vrbprefix)
      let vrbprefix=''
    endif
    if !isdirectory(redirprefix)
      let redirprefix=''
    endif
    if !isdirectory(infoprefix)
      let infoprefix=''
    endif
    let vrbfile=vrbprefix . v:servername . ".vrb"
    let redirfile=redirprefix . v:servername . ".redir"
    let viminfofile=infoprefix . v:servername . ".viminfo"
    execute 'set verbosefile=' . vrbfile
    execute 'redir >> ' . redirfile
    execute "set viminfo='50,<1000,s1000,n" . viminfofile
  endif
endif


if exists('g:maseddrop')
  execute 'tab drop ' . g:maseddrop
endif
if exists('g:masedline')
  execute g:masedline
endif

" hi ColorColumn ctermbg=lightgrey guibg=lightgrey
hi ColorColumn ctermbg=grey guibg=#035656

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

" echo '(send-window-to-workspace-from-first (get-window-by-name-re "\.jl") 2)' | sawfish-client --


nnoremap <A-1> :tabfirst<CR>
nnoremap <A-2> :tablast<CR>
let shn_doprj="shn/doprj.sh"
if filereadable(shn_doprj)
" echo "has doprj:" . shn_doprj
  nnoremap <F3>   :wa<CR>:!shn/doprj.sh i<CR>
" nnoremap <F3>   :wa<CR>:make install<CR>
  nnoremap <C-F3> :wa<CR>:!shn/doprj.sh c i<CR>
endif

nnoremap <A-F4> "=expand("%:t")<CR>p
inoremap <A-F4> <C-R>=expand("%:t")<CR>

" let shn_doall="shn/doall.sh"
" if filereadable(shn_doall)
" " echo "has doall:" . shn_doall
"   nmap <F4>   :wa<CR>:!shn/doall.sh i<CR>
"   nmap <C-F4> :wa<CR>:!shn/doall.sh c i<CR>
" endif
 
" insert filename

nmap <C-]> :call MasGoTag2()<CR>


" winpos 0 35
" winsize 155 50



" sb `=dfile`
" bd `=dfile`
" echo 'out ' . expand('<sfile>')
