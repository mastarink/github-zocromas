function shn_uni_debug ()
{
  local incore=$1
  shift
  local debugdir="$MSH_SHN_PROJECT_DIR/debug"
  local retcode=0
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin  lt rname
  local bindir libsdir

  if ! [[ $MSH_SHN_DISABLE_MARKLINE ]] ; then
     shn_msg "=>=>=>=>=>=>=>=>=>=>=>=>=>=>"
  fi
  rname=`shn_runname` || { retcode=$? ; shn_errmsg "runname: Try set MSH_SHN_RUN_NAME;rc:$retcode" ; return $retcode ; }
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
  local scmd0 scmd cmd
  local tmpdbgdir tmpdbgfile tmpdbgname tmpdbgopt
  local corename coredir="/tmp"
  local qargs
  shn_msg "$(ulimit -c)"
  for (( i=1; i <= $# ; i++ )) ; do
    qargs+=" '${!i}'"
  done
  shn_msg "to run $rname $qargs"
  if ! [[ "$runretcode" ]] || [[ "$incore" == '-' ]] ; then unset incore ; fi
  shn_msg "incore: ${incore}"
  shn_unidebug_make_tmpdbgfile $@
  shn_msg "tmpdbgfile:$tmpdbgfile"
  scmd0="${MSH_SHN_GDB:-gdb} ${MSH_SHN_GDB_OPTS} -q $bin"
  shn_msg "scmd0: ${scmd0}"
# if [[ "$tmpdbgfile" ]] ; then    tmpdbgopt=" -x $tmpdbgfile" ;  fi
  scmd="${scmd0}${tmpdbgfile:+ -x $tmpdbgfile}"
  shn_msg "scmd: ${scmd}"
  if [[ "$incore" ]] ; then
    local gid
    gid="`stat -c%g /proc/$$`"
    if corename=$( ls -1tr $coredir/*${rname}.core.$UID.$gid.* | tail -1 ) && [[ -f "$corename" ]] ; then
      shn_msg "core : $corename"
      local ltcmd="libtool --mode=execute $scmd"
      cmd="$ltcmd -c '$corename'"
    else
      shn_errmsg "not found: $corename"
      unset corename
      return 1
    fi
  elif [[ "$lt" ]] ; then
  # XXX Never here !!! XXX XXX
    if [[ "$tmpdbgfile" ]] && [[ -s "$tmpdbgfile" ]] ; then
  # libtool --mode=execute gdb -batch $binsdir/$mas_name -x $tmpdbgfile
      cmd="$ltscmd"
#	libtool --mode=execute cgdb -- -q $bin -x $tmpdbgfile
    else
      shn_errmsg "no file : $tmpdbgfile" >&2
      return 1
    fi
  else
    if [[ "$tmpdbgfile" ]] && [[ -s "$tmpdbgfile" ]] ; then
      cmd="$scmd"
#	cgdb --   -q      $bin -x $tmpdbgfile
    else
      cmd="$scmd0"
      shn_errmsg "no file : $tmpdbgfile" >&2
      return 1
#	cgdb --   -q      $bin
    fi
  fi
  if [[ "$cmd" ]] ; then
    shn_msg "eval cmd: $cmd"
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
function  shn_unidebug_make_cmddbgfile ()
{
  local suff
  cmddbgfile=""
  shn_unidebug_make_suff
  shn_msg "[suff: $suff]"
  cmddbgdir="$debugdir"
  cmddbgname="debug${incore}_${rname}${suff}.cmd"
  if [[ -d "${cmddbgdir}" ]] ; then
    cmddbgfile="${cmddbgdir}/${cmddbgname}"
    if ! [[ -f "$cmddbgfile" ]] ; then
      shn_msg "not found: $cmddbgfile"
      cmddbgname="debug${incore}_${rname}.cmd"
      cmddbgfile="${cmddbgdir}/${cmddbgname}"
    fi
    shn_msg "Using cmddbgfile: $cmddbgfile"
  fi
}
function  shn_unidebug_make_tmpdbgfile ()
{
  local cmddbgdir cmddbgfile cmddbgname sedex
  tmpdbgfile=""
  shn_unidebug_make_cmddbgfile $@
  sedex="s!^\(run\)!\1 $@!"
  shn_dbgmsg "3 cmddbgfile=$cmddbgfile"
  if [[ -f "$cmddbgfile" ]] && [[ -d "${cmddbgdir}" ]] ; then
    tmpdbgdir="${cmddbgdir}/tmp"
    tmpdbgname="${cmddbgname}.tmp"
    if ! [[ -d "${tmpdbgdir}" ]] ; then
      mkdir "${tmpdbgdir}"
    fi
    if [[ -d "${tmpdbgdir}" ]] ; then
      tmpdbgfile="${tmpdbgdir}/${tmpdbgname}"
      sed -e "$sedex" "$cmddbgfile" > $tmpdbgfile
#     shn_cat $tmpdbgfile
    fi
    shn_msg "Using tmpdbgfile: $tmpdbgfile"
  else
    shn_errmsg "not found cmddbgfile: $cmddbgfile"
  fi
}

