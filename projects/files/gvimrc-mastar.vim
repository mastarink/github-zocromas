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
   
  echo 'masedfile ok: ' . masedfile
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
  let  guienter=mas_localvimdirs . 'gvim-guienter.vim'
  if filereadable(guienter)
    au GUIEnter * sil so `=guienter`
  else
    echo 'Not found: ' . guienter
  endif

  let  vimenter=mas_localvimdirs . 'gvim-vimenter.vim'
  if filereadable(vimenter)
    au VimEnter * sil so `=vimenter`
  else
    echo 'Not found: ' . vimenter
  endif
elseif exists("masedfile")
  echo 'masedfile not found: ' . masedfile
else
  echo 'masedfile not defined'
endif
