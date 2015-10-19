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
  if ! [[ $MSH_SHN_DISABLE_MARKLINE ]] ; then
     echo "=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>" >&2
   fi
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
    for (( i=1; i <= $# ; i++ )) ; do
#     echo "$FUNCNAME $i : ${!i}" >&2
      qargs+=" '${!i}'"
    done
    shn_msg " run $rname $qargs "
    export MSH_SHN_BASHPID=$BASHPID
    export MSH_SHN_LAUNCHPID=$$
    export MSH_SHN_LAUNCHDATEM=$(datem)
    export MSH_SHN_LAUNCHDATEMT=$(datemt)
#   shn_msg "qargs:$qargs" >&2
    shn_msg " --[`datemt`]--(\$$:$$; \$BASHPID:$BASHPID;)-- "
#   shn_msg " ---- to run '$bin $1 ...' ----- "
#   shn_msg ; shn_msg ; shn_msg ; shn_msg ; shn_msg
      shn_msg ; shn_msg ; shn_msg
      shn_msg ; shn_msg ; shn_msg
#       time eval "$bin $qargs"
        if [[ -d $MSH_SHN_PROJECT_DIR/human/run ]] ; then
	  {
	    local conffile=$(find $MSH_SHN_PROJECT_DIR -name ${rname}.conf)
	    echo "# `datemt` : `daten` ++++++++++++++"
	    echo "# cat $conffile"
	    cat -n $conffile | sed -e 's@^@# => @'
	    echo "# $bin"
	    echo "# run $qargs"
	  } >> $MSH_SHN_PROJECT_DIR/human/run/history.$(/bin/date '+%Y%m%d').txt
	fi
	{
	  if pushd $MSH_SHN_CWD  &>/dev/null ; then
	    eval "$bin $qargs"
	    retcode=$?
	    popd  &>/dev/null
	  fi
	}
        if [[ -d $MSH_SHN_PROJECT_DIR/human/run ]] ; then
	  {
	    echo "# retcode:$retcode"
	    echo "# `datemt` : `daten` ------------[r]--"
	  } >> $MSH_SHN_PROJECT_DIR/human/run/history.$(/bin/date '+%Y%m%d').txt
	fi

        if ! [[ $MSH_SHN_DISABLE_MARKLINE ]] ; then
	  echo ; echo ; echo
	fi
    shn_msg " exited with $retcode "
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
  if ! [[ $MSH_SHN_DISABLE_MARKLINE ]] ; then
    echo "=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<" >&2
  fi
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

    libtool --mode=execute ${MSH_SHN_GDB:-gdb} ${MSH_SHN_GDB_OPTS} -q $bin -c "$corename"
  else
    shn_errmsg "rname:$rname"
    shn_errmsg "bsrc:$bsrc"
    shn_errmsg "MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
    shn_errmsg "debugdir:$debugdir"
  fi
  return 0
}
