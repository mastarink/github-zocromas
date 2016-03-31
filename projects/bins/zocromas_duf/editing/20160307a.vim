function! MasChomp(str)
  return substitute(a:str, '[\r\n\s]\+$', '', '' )
endfunction 

function! MasServerlist1()
  let sl=split(serverlist())
  return sl
endfunction 

function! MasServerlist2()
  let sl=split(system('gvim --serverlist'))
  return sl
endfunction 

function! MasRemote_Expr1(ser, xpr)
  return remote_expr(a:ser, a:xpr)
endfunction 

function! MasRemote_Expr2(ser, xpr)
  return system("gvim --servername '".a:ser."' --remote-expr '".a:xpr."'")
endfunction 

function! MasRemoteExpr1(ser, xpr)
" something wrong with this : returns only for "self-server"
  return MasChomp[(MasRemote_Expr1(a:ser, a:xpr))
endfunction 

function! MasRemoteExpr2(ser, xpr)
  return MasChomp(MasRemote_Expr2(a:ser, a:xpr))
endfunction 

function! MasRemoteExpr1s(ser, xpr)
  return split(MasRemote_Expr1(a:ser, a:xpr))
endfunction 

function! MasRemoteExpr2s(ser, xpr)
  return split(MasRemote_Expr2(a:ser, a:xpr))
endfunction 

function! MasListOpenFiles()
  for ser in MasServerlist1()
    let p="SHN_GVIMER_PLUS-"
    if ser=~# p 
      if ser ==# v:servername
        let wd=MasRemoteExpr1(ser, 'getcwd()')
        let tabs=MasRemoteExpr1(ser, 'tabpagenr("$")')
        let tn=1
        while tn <= tabs 
          let bl = MasRemoteExpr1s(ser, 'tabpagebuflist('.tn.')')
          for bn in bl
            echo ser[strlen(p) : ].': '.tn.'/'.bn.'=> ' . wd . '/' . MasRemoteExpr1(ser, 'buffer_name('.bn.')')
          endfor
          let tn = tn+1
        endwhile
      else
	let wd=MasRemoteExpr2(ser, 'getcwd()')
	let tabs=MasRemoteExpr2(ser, 'tabpagenr("$")')
	let tn=1
	while tn <= tabs 
"	  let wins = MasRemoteExpr2(ser, 'tabpagewinnr('.tn.",'$')")
	  let bl = MasRemoteExpr2s(ser, 'tabpagebuflist('.tn.')')
	  for bn in bl
"	    echo ser[strlen(p) : ].': '.tn.'/'.bn.'=> ' . wd . '/' . MasRemoteExpr2(ser, 'buffer_name('.bn.')')
	  endfor
"	  let w=1
"         while w <= wins
"           echo "(".tabs.") tn:".tn.":".w." - ".wins.":".MasRemoteExpr2('buffer_name(winbufnr("'.w.'"))')
"           let w = w+1
"         endwhile
	  let tn = tn+1
	endwhile
      endif
    endif
  endfor
" echo system("gvim --servername 'SHN_GVIMER_PLUS-808C04157043E1A6E9E36CC7F45EC145_' --remote-expr 'tabpagenr(".'"$"'.")'")
endfunction 

function! MasGrepMasedList( name ) 
  return split(system( 'grep -l "\(find\|sfind\|tab sfind\|e\|sp\)\s\+\<' . join(split(a:name,'\.'),'\.') . '\>" editing/mased/*.mased.vim' ))
endfunction

function! MasGrepMased( name ) 
  let mased=''
  let lpath=split(a:name, '/')
  let lmased=MasGrepMasedList(lpath[-1])
  if !empty(lmased)
    let mased=lmased[0]
  endif
  return mased
endfunction

function! MasRegisteredServername( name, mased )
  let md5=''
  if !empty(a:mased)
    let md5=toupper(split(MasChomp(system('realpath -s ' . a:mased . ' | md5sum')))[0])
  endif
  return 'SHN_GVIMER_PLUS-' . md5 . '_'
endfunction

function! MasIsResident( ses )
   let sl=MasServerlist1()
"  echo ' Index: ' . index(sl, ses)
   return index(sl, a:ses)
endfunction

function! MasEdpath( name )
  let fty=''
  let edpath=''
  let lpath=split(a:name, '/')
  echo  lpath
  let filn=lpath[-1]
  let fre='^\([[:alnum:]_]\+\)\.\([ch]\|def\)$'
  if filn =~# fre 
    let fty='c'
  endif
  if fty ==# 'c'
    let edpath='src/,mod/,src/inc/,mod/inc/'
  endif
  echo 'edpath:' . edpath
  return edpath
endfunction

function! MasRemoteEdit(name)
  let filn=a:name
  let edpath=MasEdpath( filn )
  if !filereadable( filn )
    let filn=findfile( filn, edpath)
    echo "Found:" . filn
  endif
  let filn=MasChomp(system('realpath ' . filn))
  if !empty( filn ) && filereadable( filn )  
    let mased=MasGrepMased( filn )
    let ses=MasRegisteredServername( filn, mased )
    let ind=-1
    let k=0
    let suf=''
    while k < 10 && ind < 0
      if k == 0
        let suf=''
      else
        let suf=k
      endif
      let sessuf= ses . suf
      let ind=MasIsResident( sessuf )
"     echo '(w) ind:' . ind . ' : ' . sessuf
      let k=k+1
    endwhile
"   echo "ind: " . ind
"   echo serverlist()
"   echo "ses: " . ses
    let a='gvim --servername ' . ses . ' '
    let a .= ' --cmd "let masremoteflag='       . "'" . 'yes'	. "'" . '"'
    if ind >=0
      let a .= ' --remote-tab-silent ' . filn
    else
      let a .= ' --cmd "let masedfile='       . "'" . mased	. "'" . '"'
      let a .= ' --cmd "let masedpath='       . "'" . edpath	. "'" . '"'
      let a .= ' --cmd "let mas_localvimdir=' . "'" . 'editing'	. "'" . '"'
      let a .= ' --cmd "let maseddrop='       . "'" . filn	. "'" . '"'
  "   let a .= ' --remote-tab-silent ' . a:name
    endif
    call system(  a )
  endif
endfunction

" echo MasGrepMased('duf.c')
" echo MasGrepMased('configure.ac')
" call MasRemoteEdit('Makefile.am')
"echo MasIsResident('src/duf.c')
"call  MasRemoteEdit('src/duf.c')
"call  MasRemoteEdit('src/duf_tags.h')
call  MasRemoteEdit('duf_sel_cb_node.h')
" call MasListOpenFiles()
