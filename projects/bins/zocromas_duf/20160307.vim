function! MasSearchInclude1(pre, incf)
  call cursor(1,1)
  let m=search('\(\s*#\s*include\s*<mastar/' . a:pre . '/' . a:incf . '\.h>.*$\)', 'W', '##########################################################################################')
  return m
endfunction

function! MasSearchInclude2(incf)
  call cursor(1,1)
  let m=search('^\(\s*#\s*include\s*"' . a:incf . '\.h".*$\)', 'W', '##########################################################################################')
  return m
endfunction

function! MasAddIncludeAfter1(pre, incf, apre, aftf)
  if  !MasSearchInclude1(a:pre, a:incf) && MasSearchInclude1(a:apre, a:aftf)
    let inc='#include <mastar/' . a:pre . '/' . a:incf . '.h>'
    put =inc
  endif
endfunction

function! MasAddIncludeAfter2(incf, aftf)
  if !MasSearchInclude2(a:incf) && MasSearchInclude2(a:aftf)
    let inc='#include "' . a:incf . '.h"'
    put =inc
  endif
endfunction

function! MasCommentOutInclude(incf)
  if MasSearchInclude2(a:incf)
    exec 'substitute /^\s*\(#\s*include\s*"' . a:incf . '\.h"\)/\/\* \1 \*\//'
  endif
endfunction

function! Mas20160307()
  exec '%substitute /\<\(T(\)/Q\1/ge'
" call search('duf_tracen_defs_preset')
" call search('\(duf_tracen_defs_preset.h\)')
  call MasAddIncludeAfter2('duf_errorn_defs_preset', 'duf_tracen_defs_preset')
  call MasAddIncludeAfter1('error', 'mas_error_defs', 'trace', 'mas_trace')
  call MasAddIncludeAfter1('error', 'mas_error_defs_make', 'trace','mas_trace')
  call MasAddIncludeAfter1('error', 'mas_error_defs_ctrl', 'trace','mas_trace')
  call MasCommentOutInclude('duf_tracen_defs')
  call MasCommentOutInclude('duf_errorn_defs')
  call MasCommentOutInclude('duf_start_end')
  call MasCommentOutInclude('duf_dodefs')
  call cursor(1,1)
  if search('##########################################################################################') && search('##########################################################################################')
    while search('^\(\|static\s\+\)int\(\s*MAS_UNUSED\s*\|\)\n\(duf_\|\)\([[:alpha:]][[:alnum:]_]\+\>\)(\(.*\)).*$\n{\n\s*DUF_STARTR(\s*r\s*)')
      exec 'substitute /^\(\|static\s\+\)int\(\s*MAS_UNUSED\s*\|\)\n\(duf_\|\)\([[:alpha:]][[:alnum:]_]\+\>\)(\(.*\)).*$\n{\n\(\s*DUF_STARTR(\s*r\s*)\)/\1\rSR(MOD,\4,\5)\r{\r\/\* \6 \*\//'
      normal 2-
      normal yy
      normal j
      normal %
      put! "
      normal sE
      normal A;
      exec 'substitute /\(\<MAS_UNUSED\>\)//ge'
      normal 1-
      exec 'substitute /^\(\s*DUF_ENDR(\s*r\s*);\)/\/\*\1\*\//'
    endwhile
  endif
  call cursor(1,1)

  while search('^\s*\<DOR(\s*r,\s*duf_\([[:alpha:]][[:alnum:]_]\+\)\>(')
    let cursor_pos = getcurpos()
    normal ww%x
    call setpos('.', cursor_pos)
    exec 'substitute /^\s*\<DOR(\s*r,\s*duf_\([[:alpha:]][[:alnum:]_]\+\)\>(/CR(\1,\2/e'
  endwhile
  exec '%substitute /^\s*\<\(DUF_TEST_R(\s*\(r\|rpr\|lr\)\s*);\)/\/\* \1 \*\//ge'
  exec '%substitute /\<\(DUF_NOERROR(\s*\(r\|rpr\|lr\)\s*)\)/QNOERR/ge'
  exec '%substitute /\(DUF_SQL_\)\(\(START\|END\)_STMT\|STEP\|STEPC\|CHANGES\|BIND_\(S\|LL\|LL_NZ_OPT\)\)(\(.*\)\s*\(,\s*r\>\|\<r,\)\(.*\))/\1SE_\2(\5\7\8)/ge'
  exec '%substitute /\(DUF_SQL_\)\(\(START\|END\)_STMT\|STEP\|STEPC\|CHANGES\|BIND_\(S\|LL\|LL_NZ_OPT\)\)(\(.*\)\s*\(,\s*rpr\>\|\<rpr,\)\(.*\))/\1SE_\2(\5\7\8)/ge'
  exec '%substitute /\(DUF_SQL_\)\(\(START\|END\)_STMT\|STEP\|STEPC\|CHANGES\|BIND_\(S\|LL\|LL_NZ_OPT\)\)(\(.*\)\s*\(,\s*lr\>\|\<lr,\)\(.*\))/\1SE_\2(\5\7\8)/ge'
  exec '%substitute /\<DUF_MAKE_ERRORM(\s*\(r\|rpr\|lr\),\s*DUF_ERROR_/ERRMAKE_M(/ge'
  exec '%substitute /\<DUF_MAKE_ERROR(\s*\(r\|rpr\|lr\),\s*DUF_ERROR_/ERRMAKE(/ge'
  exec '%substitute /\<DUF_IS_ERROR_N(\s*\(rpr\|lr\|r\),\s*DUF_SQL_/QISERR1_N(SQL_/ge'
" %substitute /\(DUF_SQL_\)\(\(START\|END\)_STMT\|STEPC\|CHANGES\|BIND_\(S\|LL\)\)(\(.*\)\s*\(,\s*r\|r,\)\(.*\))/1:\1;2:\2;3:\3;4:\4;5:\5;6:\6;7:\7;8:\8;)/ge
" exec '%substitute /\(DUF_MOD_DECLARE_ALL_FUNCS\)(\s*\([[:alpha:]][[:alnum:]_]\+\)\s*)/\1(duf_\2)/g'
endfunction

call Mas20160307()
