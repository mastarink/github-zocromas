function shn_runname ()
{
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin1 bin2
  if [[ "$bsrc" ]] && [[ -d "$bsrc" ]] && [[ "$MSH_SHN_PROJECT_NAME" ]]; then
    bin1="$bsrc/$MSH_SHN_PROJECT_NAME"
    bin2="$bsrc/mtest"
    if [[ -f "$bin1" ]] ; then
      shn_echon $( shn_basename $bin1 ) && return 0
    elif [[ -f "$bin2" ]] ; then
      shn_echon $( shn_basename $bin2 ) && return 0
    fi
  fi
  return 1
}
function shn_run ()
{
  local retcode=0
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin cmdfile tmpcmd sedex lt rname
  local qargs
# for (( i=1; i <= $# ; i++ )) ; do echo "$FUNCNAME $i : ${!i}" >&2 ; done
  rname=`shn_runname` || { retcode=$? ; shn_errmsg runname ; return $retcode ; }
  local bindir libsdir
  if [[ "$rname" ]] && [[ "$bsrc" ]] ; then
    libsdir="${bsrc}/.libs"
    if [[ -f "${libsdir}/${rname}" ]] ; then
      bindir="${bsrc}/.libs"
    else
      bindir="${bsrc}"
    fi
    bin=${bindir}/${rname}
    # for core dump:
    ulimit -c unlimited
    MSH_SHN_MSG_PREFIX='shn: '
    shn_msg ; shn_msg
#   shn_msg "------ to run '$bin $1 ...' -------"
    shn_msg "------ to run '$rname $1 ...' -------"
    shn_msg "------------------------------------`datemt`----"
    for (( i=1; i <= $# ; i++ )) ; do
#     echo "$FUNCNAME $i : ${!i}" >&2
      qargs+=" '${!i}'"
    done
    export MSH_SHN_BASHPID=$BASHPID
    export MSH_SHN_LAUNCHPID=$$
    shn_msg "qargs:$qargs" >&2
    shn_msg "------------------------------------`datemt`--- $$ : $BASHPID -"
    shn_msg "------ to run '$bin $1 ...' -------"
#   shn_msg ; shn_msg ; shn_msg ; shn_msg ; shn_msg

#   time eval "$bin $qargs"
    eval "$bin $qargs"

    retcode=$?
    shn_msg "returned $retcode ---------------------------------------------"
  else
    retcode=1
  fi
  if [[ "$retcode" -eq $(( 128 + 11 ))  ]] ; then
    shn_msg "gdb by exitcode= $retcode"
    
    shn G
  elif [[ "$retcode" -eq $(( 128 + 6 ))  ]] ; then
    shn_msg "gdb by exitcode= $retcode"
    
    shn G
  fi
# shn_msg "Returned $retcode"
  return $retcode
}
function shn_debug ()
{
  local retcode=0
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin cmdfile tmpcmd sedex lt rname
  rname=`shn_runname` || { retcode=$? ; shn_errmsg runname ; return $retcode ; }
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
      export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$libsdir"
    fi
    bin=${bindir}/${rname}
    cmdfile="$debugdir/debug_${rname}.cmd"
    echo "Using $cmdfile"
    sedex="s@^\(run\)@\1 $@@"
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
	libtool --mode=execute gdb        $bin -x $tmpcmd
      else
	echo "no file : $cmdfile" >&2
      fi
    else
      if [[ "$tmpcmd" ]] ; then
        gdb        $bin -x $tmpcmd
      else
        gdb        $bin
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
function shn_core_debug ()
{
  local retcode=0 corename coredir="/tmp"
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin cmdfile tmpcmd sedex lt rname
  rname=`shn_runname` || { retcode=$? ; shn_errmsg runname ; return $retcode ; }
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
      export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$libsdir"
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
    shn_msg "-=<>=--=<>=--=<>=--=<>=--=<>=--=<>=--=<>=--=<$bin>=--=<>=--=<>=--=<>=--=<>=--=<>=--=<>=--=<>=--=<>=-"
    libtool --mode=execute gdb $bin -c "$corename"
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
# rname=`shn_runname` || { retcode=$? ; shn_errmsg runname ; return $retcode ; }
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
      shn_errmsg "not found $corename" >&2
      unset corename
      return 1
    fi

    libtool --mode=execute gdb $bin -c "$corename"
  else
    shn_errmsg "rname:$rname"
    shn_errmsg "bsrc:$bsrc"
    shn_errmsg "MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
    shn_errmsg "debugdir:$debugdir"
  fi
  return 0
}
