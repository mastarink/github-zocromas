function! MasCpr()
  call cursor(1,1)
  if ! search( "THE BEGINNING", "n", 1 )
    let failed = append(0, '/* THE BEGINNING of ' . expand('%:t') . ' ' . strftime("%c") . ' */')
  endif
  if search( "BEGINNING", "n", 1 )
"   let failed = append(0, '/* THE BEGINNING */')
    call setline(1, '/* THE BEGINNING of ' . expand('%:t') . ' ' . strftime("%c") . ' */')
  endif
endfunction

function! MasGo( line, col )
" echo a:line
  call cursor( a:line, a:col )
  call foreground( )
  execute "redraw"
  return ''
endfunction
