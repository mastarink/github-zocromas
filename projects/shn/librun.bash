function shn_run ()
{


  local debugdir="$MSH_SHN_PROJECT_DIR/debug"
  local retcode=0
  local runretcode=0
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin sedex lt rname
  local bindir libsdir
  
  if ! [[ $MSH_SHN_DISABLE_MARKLINE ]] ; then
     shn_msg "=>=>=>=>=>=>=>=>=>=>=>=>=>=>"
  fi
  rname=`shn_runname` || { retcode=$? ; shn_errmsg runname rc:$retcode ; return $retcode ; }
  if [[ "$runretcode" -ne 0 ]] ; then shn_msg "-------- runretcode: $runretcode" ; fi
  if [[ "$rname" ]] && [[ "$bsrc" ]] && [[ -d "$bsrc" ]] && [[ "$MSH_SHN_PROJECT_DIR" ]] && [[ -d "$MSH_SHN_PROJECT_DIR" ]] 
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
    export MSH_SHN_BASHPID=$BASHPID
    export MSH_SHN_LAUNCHPID=$$
    export MSH_SHN_LAUNCHDATEM=$(datem)
    export MSH_SHN_LAUNCHDATEMT=$(datemt)
    shn_msg " --[`datemt`]--(\$$:$$; \$BASHPID:$BASHPID;)-- "
      shn_msg
      shn_human_run $@  ### running $bin here


        if ! [[ $MSH_SHN_DISABLE_MARKLINE ]] ; then
           shn_msg
	fi
    if [[ -f mas_debug_memory.tmp ]] ; then
##    cat mas_debug_memory.tmp >&2
      shn_msg "--=@(     exit: $retcode     )@=--          $(cat mas_debug_memory.tmp)"
      mv -f mas_debug_memory_old2.tmp  mas_debug_memory_old3.tmp
      mv -f mas_debug_memory_old.tmp  mas_debug_memory_old2.tmp
      mv -f mas_debug_memory.tmp  mas_debug_memory_old.tmp
    else
      shn_msg "--=@(     exit: $retcode     )@=--"
    fi
##  shn_msg "--=<>=--=<>=--=<$bin>=--=<>=--=<>=--"
    shn_msg "$bin"
  else
    shn_errmsg "rname:$rname"
    shn_errmsg "bsrc:$bsrc"
    shn_errmsg "MSH_SHN_PROJECT_DIR:$MSH_SHN_PROJECT_DIR"
#   shn_errmsg "debugdir:$debugdir"
    retcode=1
  fi
  shn_run_afterrun

  return $retcode
}
function shn_human_run ()
{
  local qargs
  ulimit -c unlimited  #### for core dump:
  for (( i=1; i <= $# ; i++ )) ; do
    qargs+=" '${!i}'"
  done
  shn_msg "to run $rname $qargs"
  if [[ -d $MSH_SHN_PROJECT_DIR/human/run ]] ; then
    {
      local conffile=$(find $MSH_SHN_PROJECT_DIR -name ${rname}.conf)
      echo "# `datemt` : `daten` ++++++++++++++"
      if [[ $conffile ]] && [[ -f $conffile ]] ; then
	echo "# cat $conffile"
	cat -n $conffile | sed -e 's@^@# => @'
      fi
      echo "# $bin"
      echo "# run $qargs"
    } >> $MSH_SHN_PROJECT_DIR/human/run/history.$(/bin/date '+%Y%m%d').txt
  fi
  {
    if pushd $MSH_SHN_CWD  &>/dev/null ; then
#           echo -n '◁' >&2
###   shn_msg "eval $bin $qargs"
      eval "$bin $qargs" ; retcode=$?
###   shn_msg "/eval $bin $qargs"
#           echo -n '▷' >&2
      popd  &>/dev/null
    fi
  }
  if [[ -d $MSH_SHN_PROJECT_DIR/human/run ]] ; then
    {
      echo "# retcode:$retcode"
      echo "# `datemt` : `daten` ------------[r]--"
    } >> $MSH_SHN_PROJECT_DIR/human/run/history.$(/bin/date '+%Y%m%d').txt
  fi
}
function  shn_run_afterrun ()
{
  if [[ "$retcode" -eq $(( 128 + 11 ))  ]] ; then
    shn_msg "gdb by exitcode= $retcode"
    
    local runretcode=$retcode
    shn G
  elif [[ "$retcode" -eq $(( 128 + 6 ))  ]] ; then
    shn_msg "gdb by exitcode= $retcode"
    local runretcode=$retcode
    shn G
  fi
# shn_msg "Returned $retcode"
  if ! [[ $MSH_SHN_DISABLE_MARKLINE ]] ; then
    shn_msg "=<=<=<=<=<=<=<=<=<=<=<" >&2
  fi
}
function shn_runname ()
{
  local bsrc="${MSH_SHN_DIRS[buildsrc]}"
  local bin
  if  [[ "$bsrc" ]] && [[ -d "$bsrc" ]] ; then
    if [[ "$MSH_SHN_RUN_NAME" ]] ; then
      bin="$bsrc/$MSH_SHN_RUN_NAME"
##    shn_msg "bin: $bin (MSH_SHN_RUN_NAME)"
    elif [[ "$MSH_SHN_PROJECT_NAME" ]] ; then
      bin="$bsrc/$MSH_SHN_PROJECT_NAME"
##    shn_msg "bin: $bin (MSH_SHN_PROJECT_NAME)"
    fi
  fi
  if [[ $bin ]] && [[ -f "$bin" ]] && [[ -x "$bin" ]] ; then
    shn_echon $( shn_basename $bin ) && return 0
  fi
  return 1
}

