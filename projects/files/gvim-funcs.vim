" echo 'in ' . expand('<sfile>')
" echo "+++++++++++++++++++++++++++++++++ : " . expand("<sfile>")
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

function! MasGoTag ()
  exec 'tag ' . expand('<cword>')
  let cup=getpos('.')
  let bn=bufnr('%')
" exec 'normal '
" echo expand('<cword>')
" normal bywp
" exec 'normal '
  pop
  exec 'tab drop #' . bn
  call setpos('.', cup)
endfunction


"/* CTRL-] to go to the tag                                               */
"/* Save the cursor position to a variable using getpos()                 */
"/* CTRL-^ (or CTRL-6) to go back to the file you were editing before     */
"/* :tab sb # to either open a new tab with the buffer you jumped to with */
"/* CTRL-6, or (with switchbuf set properly) jump to the already-open tab */
"/* Set the cursor position to the saved position using setpos()          */



