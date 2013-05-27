function mas_debug ()
{
  local sedex cmdfile tmpcmd lt dname binsdir bin libsdirs

  ulimit -c

  if [[ -f "${build_at}/src/.libs/${rname_preset}" ]] ; then
    binsdir="${build_at}/src/.libs"
  else
    binsdir="${build_at}/src"
  fi
  libsdirs=$binsdir
  export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$libsdirs"
  bin=${binsdir}/${rname_preset}
  cmdfile="$debugdir/debug_${rname_preset}.cmd"
  sedex="s@^\(run\)@\1 $@@"

  tmpcmd="${cmdfile}.tmp"
  sed -e "$sedex" "$cmdfile" > $tmpcmd
  cat $tmpcmd
  echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH" >&2

# exit
  if [[ "$lt" ]] ; then
    if [[ -f "$tmpcmd" ]] && [[ -s "$tmpcmd" ]] ; then
  # libtool --mode=execute gdb -batch $binsdir/$mas_name -x $tmpcmd
      libtool --mode=execute gdb        $bin -x $tmpcmd
    else
      echo "no file : $cmdfile" >&2
    fi
  else
    gdb        $bin -x $tmpcmd
  fi
}

function gdb_core_any ()
{
  local  dname bname builddir binsdir libsdirs corename

    if corename=$( ls -1tr /tmp/*${rname_preset}.core.$UID.$UID.* | tail -1 ) && [[ -f "$corename" ]] ; then
      echo "$LINENO $corename" >&2
    else
      echo "$LINENO $corename" >&2
      unset corename
    fi
# echo "mas_name: $mas_name" >&2
# echo "bname: $bname" >&2
# echo "dname: $dname" >&2
# echo "builddir: $builddir" >&2
# echo "binsdir: $binsdir" >&2


# export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$libsdirs"
  ulimit -c

  if [[ -f "$corename" ]] && [[ -x $binsdir/$dname ]] ; then
    libtool --mode=execute gdb $binary_preset -c "$corename"
  elif [[ -f "$corename" ]] && [[ -x $binsdir/$dname ]] ; then
    libtool --mode=execute gdb $binary_preset -c "$corename"
  else
    echo "no file(s)  $binary_preset ( $corename )">&2
  fi
}
function debug_any ()
{
#   clear && 
  make_any && mas_debug $@
}








