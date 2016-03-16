function! MasRemote()
  let mas_servers=split(serverlist())
  let sss=''
  let vx=''
" call filter(mas_servers, 'v:val !~ "^GVIM"')
  for ms in mas_servers
"  tabpagebuflist
    echo " A ". ms
    let v = remote_expr(ms, "tabpagenr('$')" )
    echo " B ". ms . " - " . v
    let vx = vx . ' ;' . v
    let sss = sss . "," . ms
  endfor
  echo len(mas_servers) . ' : ' . vx 
  \ . ' - ' . sss
endfunction
call MasRemote()

