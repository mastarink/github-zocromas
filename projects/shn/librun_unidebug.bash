function shn_uni_debug ()
{
  local incore=$1
  shift
  local retcode=0 corename coredir="/tmp"
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin cmdfile tmpcmd sedex lt rname scmd0 scmd ltcmd cmd
  rname=`shn_runname` || { retcode=$? ; shn_errmsg runname rc:$retcode ; return $retcode ; }
  local bindir libsdir debugdir
  local gid
  gid="`stat -c%g /proc/$$`"
  shn_msg "runretcode: $runretcode"
  local suff
  if [[ "$runretcode" -eq $(( 128 + 11 )) ]] ; then
    suff='_segfault'
  elif [[ "$runretcode" -eq $(( 128 + 6 )) ]] ; then
    suff='_assert'
  elif [[ "$runretcode" ]] ; then
    suff="_${runretcode}"
  fi
  debugdir="$MSH_SHN_PROJECT_DIR/debug"
  libsdir="${bsrc}/.libs"
  shn_dbgmsg "1 $FUNCNAME"
  if [[ "$rname" ]] && [[ "$bsrc" ]] && [[ -d "$bsrc" ]] && [[ "$MSH_SHN_PROJECT_DIR" ]] && [[ -d "$MSH_SHN_PROJECT_DIR" ]] && [[ -d "$debugdir" ]] ; then
    shn_dbgmsg "2 $FUNCNAME"
    if [[ -d "$libsdir" ]] ; then
      if [[ -f "${libsdir}/${rname}" ]] ; then
        bindir="$libsdir"
      else
        bindir="$bsrc"
      fi
      export LD_LIBRARY_PATH="$libsdir"
#     export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$libsdir"
    fi
    bin=${bindir}/${rname}
    if ! [[ "$runretcode" ]] || [[ "$incore" == '-' ]] ; then unset incore ; fi
    cmdfile="$debugdir/debug${incore}_${rname}${suff}.cmd"
    if ! [[ -f "$cmdfile" ]] ; then
      shn_msg "not found: $cmdfile"
      cmdfile="$debugdir/debug${incore}_${rname}.cmd"
    fi
    echo "Using $cmdfile"
    sedex="s!^\(run\)!\1 $@!"
    shn_dbgmsg "3 cmdfile=$cmdfile"
    if [[ -f "$cmdfile" ]] ; then
      tmpcmd="${cmdfile}.tmp"
      /bin/sed -e "$sedex" "$cmdfile" > $tmpcmd
      shn_cat $tmpcmd
      shn_dbgmsg "3 tmpcmd=$tmpcmd"
    else
      shn_msg "not found: $cmdfile"
    fi

    ulimit -c
    scmd0="${MSH_SHN_GDB:-gdb} ${MSH_SHN_GDB_OPTS}  -q      $bin"
    if [[ "$tmpcmd" ]] ; then
      scmd="$scmd0 -x $tmpcmd"
    else
      scmd="$scmd0"
    fi
    ltcmd="libtool --mode=execute $scmd"
    if [[ "$incore" ]] ; then
      if corename=$( ls -1tr $coredir/*${rname}.core.$UID.$gid.* | tail -1 ) && [[ -f "$corename" ]] ; then
	shn_msg "core : $corename"
	cmd="$ltcmd -c '$corename'"
      else
	shn_errmsg "not found: $corename"
	unset corename
	return 1
      fi
    elif [[ "$lt" ]] ; then
      if [[ "$tmpcmd" ]] && [[ -s "$tmpcmd" ]] ; then
    # libtool --mode=execute gdb -batch $binsdir/$mas_name -x $tmpcmd
	cmd="$ltscmd"
#	libtool --mode=execute cgdb --  -q       $bin -x $tmpcmd
      else
	echo "no file : $tmpcmd" >&2
	return 1
      fi
    else
      if [[ "$tmpcmd" ]] ; then
	cmd="$scmd"
#	cgdb --   -q      $bin -x $tmpcmd
      else
	cmd="$scmd0"
#	cgdb --   -q      $bin
      fi
    fi
    if [[ "$cmd" ]] ; then
      shn_msg "$cmd"
      eval "$cmd"
    fi
    shn_msg "--=<>=--=<>=--=<>=--=<>=--=<$bin>=--=<>=--=<>=--=<>=--=<>=--"
  else
    shn_errmsg "rname:$rname"
    shn_errmsg "bsrc:$bsrc"
    shn_errmsg "MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
    shn_errmsg "debugdir:$debugdir"
  fi
  return 0
}

