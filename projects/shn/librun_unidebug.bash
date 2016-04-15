function shn_uni_debug ()
{
  local incore=$1
  shift
  local debugdir="$MSH_SHN_PROJECT_DIR/debug"
  local retcode=0
  local runretcode=0
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin sedex lt rname
  local bindir libsdir

  if ! [[ $MSH_SHN_DISABLE_MARKLINE ]] ; then
     echo "=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>" >&2
  fi
  rname=`shn_runname` || { retcode=$? ; shn_errmsg runname rc:$retcode ; return $retcode ; }
  if [[ "$runretcode" ]] ; then shn_msg "-------- runretcode: $runretcode" ; fi
  if [[ "$rname" ]] && [[ "$bsrc" ]] && [[ -d "$bsrc" ]] && [[ "$MSH_SHN_PROJECT_DIR" ]] && [[ -d "$MSH_SHN_PROJECT_DIR" ]] && [[ -d "$debugdir" ]]
	then
    libsdir="${bsrc}/.libs"
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
   
    MSH_SHN_MSG_PREFIX='shn: '
    shn_msg ; shn_msg
    export MSH_SHN_BASHPID=$BASHPID
    export MSH_SHN_LAUNCHPID=$$
    export MSH_SHN_LAUNCHDATEM=$(datem)
    export MSH_SHN_LAUNCHDATEMT=$(datemt)
    shn_msg " --[`datemt`]--(\$$:$$; \$BASHPID:$BASHPID;)-- "
      shn_msg ; shn_msg ; shn_msg
      shn_msg ; shn_msg ; shn_msg
      shn_debug_run $@  ### running $bin here

        if ! [[ $MSH_SHN_DISABLE_MARKLINE ]] ; then
	  echo ; echo ; echo
	fi
    if [[ -f mas_debug_memory.tmp ]] ; then
      cat mas_debug_memory.tmp >&2
      mv -f mas_debug_memory_old2.tmp  mas_debug_memory_old3.tmp
      mv -f mas_debug_memory_old.tmp  mas_debug_memory_old2.tmp
      mv -f mas_debug_memory.tmp  mas_debug_memory_old.tmp
    fi
    shn_msg "exited with $retcode "
    shn_msg "--=<>=--=<>=--=<>=--=<>=--=<$bin>=--=<>=--=<>=--=<>=--=<>=--"
  else
    shn_errmsg "rname:$rname"
    shn_errmsg "bsrc:$bsrc"
    shn_errmsg "MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
    shn_errmsg "debugdir:$debugdir"
    retcode=1
  fi
  # don't in debug! : shn_run_afterrun

  return $retcode
}
function shn_debug_run ()
{
  local scmd0 scmd ltcmd cmd  gid tmpcmd
  local corename coredir="/tmp"
  local qargs
  ulimit -c
  for (( i=1; i <= $# ; i++ )) ; do
    qargs+=" '${!i}'"
  done
  shn_msg "to run $rname $qargs"
  if ! [[ "$runretcode" ]] || [[ "$incore" == '-' ]] ; then unset incore ; fi
  shn_unidebug_make_tmpcmdfile $@
  shn_msg "tmpcmd:$tmpcmd"
  scmd0="${MSH_SHN_GDB:-gdb} ${MSH_SHN_GDB_OPTS}  -q      $bin"
  if [[ "$tmpcmd" ]] ; then
    scmd="$scmd0 -x $tmpcmd"
  else
    scmd="$scmd0"
  fi
  ltcmd="libtool --mode=execute $scmd"
  gid="`stat -c%g /proc/$$`"
  shn_msg "incore: $incore"
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
}
function  shn_unidebug_make_suff ()
{
  if [[ "$runretcode" -eq $(( 128 + 11 )) ]] ; then
    suff='_segfault'
  elif [[ "$runretcode" -eq $(( 128 + 6 )) ]] ; then
    suff='_assert'
  elif [[ "$runretcode" ]] ; then
    suff="_${runretcode}"
  fi
}
function  shn_unidebug_make_cmdfile ()
{
  local suff
  shn_unidebug_make_suff
  shn_msg "[suff: $suff]"
  cmdfile="$debugdir/debug${incore}_${rname}${suff}.cmd"
  if ! [[ -f "$cmdfile" ]] ; then
    shn_msg "not found: $cmdfile"
    cmdfile="$debugdir/debug${incore}_${rname}.cmd"
  fi
  echo "Using $cmdfile"
}
function  shn_unidebug_make_tmpcmdfile ()
{
  local cmdfile
  shn_unidebug_make_cmdfile
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
}

