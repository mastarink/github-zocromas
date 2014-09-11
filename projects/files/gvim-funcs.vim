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

function! MasGoTag2 ()
  let tl=taglist(expand('<cword>'))
  call system('ctags src/*.c src/inc/*.h')
  try
    for t in tl
      let bn = bufnr( t.filename )
"     echo t.name . ' ' . bn 
"     echo t.cmd
      if bn > 0
	try
	  execute 'tab drop #' . bn
	catch
	  echoerr "MasGoTag2: tab drop error"
	endtry
"       if search( t.cmd ) > 0
"         echo "A: " . t.cmd
"       else
"         echo "B: " . t.cmd
"       endif
        echo "X: " . t.cmd
	let mmagic=&magic
	let mwrapscan=&wrapscan
	let mignorecase=&ignorecase
	set wrapscan
	set nomagic
	set noignorecase
	execute t.cmd
	let &magic=mmagic
	let &wrapscan=mwrapscan
	let &ignorecase=mignorecase
      else
	echoerr "MasGoTag2: not in this set 1"
      endif
      break
    endfor
  catch
    echoerr "MasGoTag2: not in this set 2"
  endtry
endfunction

function! MasGoTag1 ()
  try
    exec 'tag ' . expand('<cword>')
      let cup=getpos('.')
      let bn=bufnr('%')
"     exec 'normal '
"     echo expand('<cword>')
"     normal bywp
"     exec 'normal '
    pop
    echo "buffer" . bn
    if bn > 0
      try
	exec 'tab drop #' . bn
	call setpos('.', cup)
      catch
	echoerr "MasGoTag1 tab drop error"
      endtry
    else
      echoerr "not in this set"
    endif
  catch
    echoerr "MasGoTag1 tag error"
    pop
  endtry
endfunction


"/* CTRL-] to go to the tag                                               */
"/* Save the cursor position to a variable using getpos()                 */
"/* CTRL-^ (or CTRL-6) to go back to the file you were editing before     */
"/* :tab sb # to either open a new tab with the buffer you jumped to with */
"/* CTRL-6, or (with switchbuf set properly) jump to the already-open tab */
"/* Set the cursor position to the saved position using setpos()          */



