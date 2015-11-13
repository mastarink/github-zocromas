version 6.0
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
inoremap <M-F4> =expand("%:t")
map! <Undo> i
map! <S-Insert> <MiddleMouse>
inoremap <F6> =strftime(mas_timefmt)
nmap 	 w
nmap  :call MasGoTag2()
map <Redo> i
map Q gq
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
vnoremap p :let current_reg = @"gvdi=current_reg
nnoremap <M-F4> "=expand("%:t")p
nnoremap <C-F3> :wa:!shn/doprj.sh c i
nnoremap <F3> :wa:!shn/doprj.sh i
vmap <F8> !boxes -r -dc-cmt
vmap <F7> !boxes -dc-cmt
nmap <F8> !$boxes -r -dc-cmt
omap <F8> !$boxes -r -dc-cmt
nmap <F7> !$boxes -dc-cmt
omap <F7> !$boxes -dc-cmt
map <F2> mfgggqG`f
map <Undo> i
map <S-Insert> <MiddleMouse>
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(expand((exists("g:netrw_gx")? g:netrw_gx : '<cfile>')),netrw#CheckIfRemote())
nnoremap <F6> "=strftime(mas_timefmt)p
nmap <kEnter> :set wrap!
nmap <S-Tab> W
nmap <kMinus> 1_
nmap <kPlus> _
map <F1> :editGN
map! <Redo> i
nnoremap ² :tablast
nnoremap ± :tabfirst
cabbr gitv =(getcmdtype()==':' && getcmdpos()==1 ? 'Gitv' : 'gitv')
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set background=dark
set backspace=indent,eol,start
set cmdheight=2
set noequalalways
set exrc
set fileencodings=utf8
set fileformats=unix,mac,dos
set formatoptions=tcql
set formatprg=~/.mas/bin/indent-c
set guifont=Iosevka\ 12
set guioptions=aegimrLt
set guitablabel=%M%R\ %t
set history=500
set hlsearch
set incsearch
set laststatus=2
set lazyredraw
set makeprg=make\ -C\ ~/.mas/lib/big/misc/develop/autotools/zoc/projects/bins/zocromas_duf/.auxdir/.build
set matchtime=2
set mouse=a
set mousemodel=popup
set path=src/,mod/,src/inc/,mod/inc/,inc/,settling/src/,settling/src/inc
set printfont=Liberation\ Mono\ 8
set ruler
set runtimepath=~/.vim,/usr/share/vim/vimfiles,/usr/share/vim/vim74,/usr/share/vim/vimfiles/after,~/.vim/after,vim_user_dir,~/.mas/config/vim
set scrolloff=7
set shiftwidth=2
set showmatch
set sidescroll=1
set statusline=%<%f\ %h%m%r%=\ d%b:x%B\ %y\ %-14.(%l,%c%V%)\ #%o\ %P
set suffixes=.bak,~,.o,.h,.info,.swp,.obj,.info,.aux,.log,.dvi,.bbl,.out,.o,.lo
set switchbuf=usetab,newtab,split
set termencoding=utf-8
set textwidth=78
set undolevels=10000
set verbosefile=editing/.gvimvrb/SHN_GVIMER_PLUS-28058D5455633722AE10A7ECEF833218.vrb
set viminfo='50,<1000,s1000,nediting/.gvimvrb/SHN_GVIMER_PLUS-28058D5455633722AE10A7ECEF833218.viminfo
set virtualedit=block
set window=29
set winheight=25
set winminheight=0
" vim: set ft=vim :
