set ttyfast
" set notbi
set tbi

" if !exists("masedfile")
"   let dfile=expand('%')
" " dfile : src.mased
"   if empty(dfile)
"     echo "Not set dfile "
"     echo "Not exists masedfile"
"   else
"     let masedfile='mased/' . dfile . '.vim'
"   endif
" endif


if exists("masedfile") && !empty(masedfile) && filereadable(masedfile)
   
"20160308.155851 echo 'masedfile ok: ' . masedfile

" if exists('v:servername')
"   echo '(before) v:servername: ' . v:servername
" endif
" if exists('g:maseddrop')
"   echo '(before) g:maseddrop: ' . g:maseddrop
" endif
" if exists('g:maseddrop')
"   echo '(before) g:masedpath: ' . g:masedpath
" endif


" set viminfo='50,<1000,s1000,.viminfo
  
" set path=src,src/inc
" tab new
  if exists("masedpath") && !empty(masedpath)
"   echo "set path=" . masedpath
    execute 'set path=' . masedpath
  endif
  sil so `=masedfile`

" au ColorScheme sil so `=masedfile`
" autocmd! VimEnter * source gvim-vimenter.vim

" au GUIEnter * winsize 156 50
  if !exists("mas_localvimdir") || empty(mas_localvimdir)
"   let mas_localvimdir='./'
    let mas_localvimdir=getcwd()
  endif
  if !empty(mas_localvimdir)
    let mas_localvimdirs=mas_localvimdir . '/'
  endif
" let  guienter=mas_localvimdirs . 'gvim-guienter.vim'
" if filereadable(guienter)
"   echo 'guienter: ' . guienter
"   au GUIEnter * sil so `=guienter`
" else
"   echo 'not found guienter: ' . guienter
" endif
" let  tabenter=mas_localvimdirs . 'gvim-tabenter.vim'
" if filereadable(tabenter)
"   echo 'tabenter: ' . tabenter
"   au TabEnter * sil so `=tabenter`
" else
"   echo 'not found tabenter: ' . tabenter
" endif
" let  bufenter=mas_localvimdirs . 'gvim-bufenter.vim'
" if filereadable(bufenter)
"   echo 'bufenter: ' . bufenter
"   au BufEnter * sil so `=bufenter`
" else
"   echo 'not found bufenter: ' . bufenter
" endif
" let  bufreadpost=mas_localvimdirs . 'gvim-bufreadpost.vim'
" if filereadable(bufreadpost)
"   echo 'bufreadpost: ' . bufreadpost
"   au BufReadPost * sil so `=bufreadpost`
" else
"   echo 'not found bufreadpost: ' . bufreadpost
" endif
" let  bufwinenter=mas_localvimdirs . 'gvim-bufwinenter.vim'
" if filereadable(bufwinenter)
"   echo 'bufwinenter: ' . bufwinenter
"   au BufWinEnter * sil so `=bufwinenter`
" else
"   echo 'not found bufwinenter: ' . bufwinenter
" endif
" let  winenter=mas_localvimdirs . 'gvim-winenter.vim'
" if filereadable(winenter)
"   echo 'winenter: ' . winenter
"   au WinEnter * sil so `=winenter`
" else
"   echo 'not found winenter: ' . winenter
" endif
"
" let  mfiletype=mas_localvimdirs . 'gvim-filetype.vim'
" if filereadable(mfiletype)
"   echo 'mfiletype: ' . mfiletype
"   au FileType * sil so `=mfiletype`
" else
"   echo 'not found mfiletype: ' . mfiletype
" endif

  let  vimenter=mas_localvimdirs . 'gvim-vimenter.vim'
  if filereadable(vimenter)
"20160308.155917   echo 'vimenter: ' . vimenter
    au VimEnter * sil so `=vimenter`
  else
    echo 'not found vimenter: ' . vimenter
  endif
elseif exists("masedfile")
  echo 'not found masedfile: ' . masedfile
else
  echo 'not defined masedfile: '
endif
