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
  
" echo 'masedfile: ' . masedfile
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
  

  execute 'set path=' . g:masedpath
  sil source `=masedfile`

" au ColorScheme sil so `=masedfile`
" autocmd! VimEnter * source gvim-vimenter.vim

" au GUIEnter * winsize 156 50
  
  if filereadable("gvim-guienter.vim")
    au GUIEnter * source gvim-guienter.vim
  endif
  if filereadable("gvim-vimenter.vim")
    au VimEnter * source gvim-vimenter.vim
  endif
endif
