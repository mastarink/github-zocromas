function shn_core_debug ()
{
  local retcode=0 corename coredir="/tmp"
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin cmdfile tmpcmd sedex lt rname
  rname=`shn_runname` || { retcode=$? ; shn_errmsg runname rc:$retcode ; return $retcode ; }
  local bindir libsdir debugdir
  local gid
  gid="`stat -c%g /proc/$$`"

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

    ulimit -c
    if corename=$( ls -1tr $coredir/*${rname}.core.$UID.$gid.* | tail -1 ) && [[ -f "$corename" ]] ; then
      shn_msg "core : $corename"
    else
      shn_errmsg "not found $corename"
      unset corename
      return 1
    fi
    shn_msg "--=<>=--=<>=--=<>=--=<>=--=<$bin>=--=<>=--=<>=--=<>=--=<>=--"
    shn_msg "libtool --mode=execute ${MSH_SHN_GDB:-gdb} ${MSH_SHN_GDB_OPTS} -q $bin -c '$corename'"
    libtool --mode=execute ${MSH_SHN_GDB:-gdb} ${MSH_SHN_GDB_OPTS} -q $bin -c "$corename"
  else
    shn_errmsg "rname:$rname"
    shn_errmsg "bsrc:$bsrc"
    shn_errmsg "MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
    shn_errmsg "debugdir:$debugdir"
  fi
  return 0
}
function shn_core_debug_installed ()
{
  local retcode=0 corename coredir="/tmp"
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin cmdfile tmpcmd sedex lt rname
  rname=$1
  shift
# rname=`shn_runname` || { retcode=$? ; shn_errmsg runname rc:$retcode ; return $retcode ; }
  local bindir libsdir debugdir
  local gid
  gid="`stat -c%g /proc/$$`"
  bindir="/usr/bin"

  debugdir="$MSH_SHN_PROJECT_DIR/debug"
  shn_dbgmsg "1 $FUNCNAME"
  if [[ "$rname" ]] && [[ "$bsrc" ]] && [[ -d "$bsrc" ]] && [[ "$MSH_SHN_PROJECT_DIR" ]] && [[ -d "$MSH_SHN_PROJECT_DIR" ]] && [[ -d "$debugdir" ]] ; then
    shn_dbgmsg "2 $FUNCNAME"
    bin=${bindir}/${rname}

    ulimit -c
    if corename=$( ls -1tr $coredir/*${rname}.core.$UID.$gid.* | tail -1 ) && [[ -f "$corename" ]] ; then
      shn_msg "core : $corename" >&2
    else
      shn_errmsg "not found $corename"
      unset corename
      return 1
    fi

    libtool --mode=execute ${MSH_SHN_GDB:-gdb} ${MSH_SHN_GDB_OPTS} -q $bin -c "$corename"
  else
    shn_errmsg "rname:$rname"
    shn_errmsg "bsrc:$bsrc"
    shn_errmsg "MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
    shn_errmsg "debugdir:$debugdir"
  fi
  return 0
}
function shn_debug ()
{
  local retcode=0
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin cmdfile tmpcmd sedex lt rname
  rname=`shn_runname` || { retcode=$? ; shn_errmsg runname rc:$retcode ; return $retcode ; }
  local bindir libsdir debugdir
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
    cmdfile="$debugdir/debug_${rname}.cmd"
    echo "Using $cmdfile"
    sedex="s!^\(run\)!\1 $@!"
    if [[ -f "$cmdfile" ]] ; then
      tmpcmd="${cmdfile}.tmp"
      /bin/sed -e "$sedex" "$cmdfile" > $tmpcmd
      shn_cat $tmpcmd
    fi
    shn_dbgmsg "3 cmdfile=$cmdfile"
    shn_dbgmsg "3 tmpcmd=$tmpcmd"

    ulimit -c

    if [[ "$lt" ]] ; then
      if [[ "$tmpcmd" ]] && [[ -s "$tmpcmd" ]] ; then
    # libtool --mode=execute gdb -batch $binsdir/$mas_name -x $tmpcmd
	libtool --mode=execute ${MSH_SHN_GDB:-gdb} ${MSH_SHN_GDB_OPTS}  -q       $bin -x $tmpcmd
#	libtool --mode=execute cgdb --  -q       $bin -x $tmpcmd
      else
	echo "no file : $cmdfile" >&2
      fi
    else
      if [[ "$tmpcmd" ]] ; then
	${MSH_SHN_GDB:-gdb} ${MSH_SHN_GDB_OPTS}  -q      $bin -x $tmpcmd
#	cgdb --   -q      $bin -x $tmpcmd
      else
	${MSH_SHN_GDB:-gdb} ${MSH_SHN_GDB_OPTS}   -q      $bin
#	cgdb --   -q      $bin
      fi
    fi
  else
    shn_errmsg "rname:$rname"
    shn_errmsg "bsrc:$bsrc"
    shn_errmsg "MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
    shn_errmsg "debugdir:$debugdir"
  fi
  return 0
}
