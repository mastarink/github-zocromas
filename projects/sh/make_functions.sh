function make_m ()
{
  local ername 
  ername="$make_errdir/make_m.`datemt`.${MAS_MAKE_CNT}.result"
  MAS_MAKE_CNT=$(( $MAS_MAKE_CNT + 1 ))
  if [[ "$build_at" ]] && pushd "$build_at" >/dev/null ; then
    if [[ "$make_errdir" ]] && ! [[ -d "$make_errdir" ]] ; then
      if ! mkdir "$make_errdir" ; then
        popd >/dev/null
	echo "$LINENO ERROR make_m" >&2
	return 1
      fi
    fi
    if [[ "$ername" ]] && ! [[ -f "$ername" ]] ; then
#     echo ">>>>>>>>>>>>>> $( realpath $ername )" >&2
      if make -s $@  >$ername 2>&1 ; then
        setup_vers || return 1
#	rm $ername
  #     echo "$LINENO GOOD make_m" >&2
  	popd >/dev/null && return 0
	echo "$LINENO ERROR make_m" >&2
	return 1
      elif [[ -s "$ername" ]] ; then
	echo -n "$ername >>>>>>>>" >&2
	if false ; then
	  cat $ername
	else
	  grep -v '^Making all in ' $ername >&2
	fi
	echo -n "<<<<<<<<" >&2
      # grep error $ername | head -1 | sed -ne 's/^\([a-z]\+\.[ch]\):\([[:digit:]]\+\):.*$/\1 :: \2/p'
      else
	echo "$LINENO ERROR make_m ; ername:$ername" >&2
      fi
    else
      echo "$LINENO ($make_cnt) ERROR make_m ; ername:$ername exists" >&2
      cat $ername
    fi
  else
    echo "$LINENO ERROR make_m" >&2
  fi
  popd  >/dev/null
  echo "$LINENO ERROR make_m" >&2
  return 1
}
function grep_make_m ()
{
# echo "grep_make_m" >&2
# grep  --include='*.[ch]' -r '\<\(pipe\|fork\|waitpid\|accept\|connect\|bind\|getsockopt\|setsockopt\|inet_ntop\|listen\|close\|mkfifo\)\>\s*(' src inc | grep -v '^src/mas_lib[0-9]*\.c'
  if [[ -f "grepfunexclude.txt" ]] ; then
  ! grep -r --color --include='*.[ch]' --exclude-from="grepfunexclude.txt" -r '\<\(pipe\|fork\|waitpid\|accept\|connect\|bind\|getsockopt\|setsockopt\|inet_ntop\|listen\|close\|mkfifo\)\>\s*(' src && \
  ! grep -r --color --include='*.[ch]' --exclude-from="grepfunexclude.txt" -r '\<\(malloc\|free\|strdup\|strndup\)\>\s*(' src && \
    make_m
  else
    make_m
  fi
# make -s ctags
}
function make_any ()
{
# echo "make_any" >&2
  time grep_make_m
}
function make_any_tilme ()
{
  local timname tstart
  timname=make-timing.`datemt`.result
  if [[ "$timname" ]] ; then
    if ! [[ -f "$timname" ]] ; then
      tstart=`date`
      if make_any $@ > "$timname" 2>&1 ; then
#   pwd >&2
#   echo "timname:$timname" >&2
	echo "$tstart; $( grep '^real' $timname | sed -ne 's/^real\s*//p' )" >> $make_logfile
	cat "$timname"
	rm "$timname"
      else
	echo "$tstart; $( grep '^real' $timname | sed -ne 's/^real\s*//p' )" >> $make_logfile
	cat "$timname"
	rm "$timname"
	return 1
      fi
    fi
  fi
  return 0
}
function make_target ()
{
  local ilog target
  target=$1
  shift
# echo "install [$prjname]" >&2
  ilog="/tmp/$target-$prjname.log"
  if [[ "$build_at" ]] ; then
    if pushd "$build_at" >/dev/null ; then
      if ! make -s $target > $ilog ; then
	zoc_error "$LINENO" "${BASH_SOURCE[0]}" "$prjname $target FAIL : $ilog"
#	cat "$ilog" >&2
	popd  >/dev/null
	return 1
      fi
      popd >/dev/null || return 1
    else
      return 1
    fi
  else
    return 1
  fi
  return 0
}

