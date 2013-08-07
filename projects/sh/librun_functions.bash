function run_installed ()
{
  local cmd_exec
  local binary builddir bname rname mcaller
  mcaller=$1
  shift
  rname=$( basename $mcaller )

  if [[ $rname =~ run_([a-z]+)\.sh ]] ; then
    bname=${BASH_REMATCH[1]}
    rname="${binprefix}$bname"
  elif [[ $rname =~ ^([a-z]+)$ ]] ; then
    bname=${BASH_REMATCH[1]}
    rname="${binprefix}$bname"
  fi

  binary="$flavourdir/bin/$rname"

# echo "mcaller:$mcaller" >&2
# echo "rname:$rname" >&2
# echo "bname:$bname" >&2
# echo "flavourdir:$flavourdir" >&2
# echo "binary:$binary" >&2
# echo "MAS_ZOCROMAS_HERE:$MAS_ZOCROMAS_HERE" >&2

  if [[ "$MAS_SOURCED" ]] ; then
    echo "MAS_SOURCED: $MAS_SOURCED" >&2
    cmd_exec=eval
  else
    cmd_exec=exec
  fi
  if [[ "$binary" ]]; then
     echo "<<< $rname >>>" >&2
#      env | grep MAS_ZOCROMAS >&2

  # for core dump:
     ulimit -c unlimited

#     make_any && usleep 500000 && clear && $cmd_exec $builddir/$rname "$@"
#      echo "bash:to run  $builddir/$rname" >&2
     echo "bash:starting $MAS_ZOCROMAS_HERE : $binary" >&2
     if type nanosleep >/dev/null 2>&1 ; then
       make_any && nanosleep 0.5 && $cmd_exec $binary "$@"
     elif type usleep >/dev/null 2>&1 ; then
       make_any && usleep 50000 && $cmd_exec $binary "$@"
     else
       make_any && $cmd_exec $binary "$@"
     fi
     echo "bash:$MAS_ZOCROMAS_HERE exited" >&2
  fi
}
function run_any ()
{
  local cmd_exec binary bin_dir bin_name
  # for core dump:
  ulimit -c unlimited
  
  
  if [[ "$MAS_RUN_INSTALLED" ]] ; then
    binary="$ibinary_preset"
  else
    binary=$rbinary_preset
  fi
  if ! [[ "$binary" ]] ; then
    echo "binary not set to run [ibinary_preset: $ibinary_preset ; rbinary_preset: $rbinary_preset]" >&2
    return 1
  fi
  bin_dir=`dirname $binary`
  bin_name=`basename $binary`
  bin_run="./$bin_name"
  echo "To run [$binary] [$bin_run] [$bin_name]" >&2

# echo "To RUN rbinary: $binary" >&2
  #     make_any && usleep 500000 && clear && $cmd_exec $builddir/$rname "$@"
  #      echo "bash:to run  $builddir/$rname" >&2

  if [[ "$MAS_USE_RUN_STRACE" ]] ; then
#   straceit='strace -q -fr -C -o strace.tmp'
    straceit="strace -q -fr -C -o ${MAS_PROJECTS_TMPDIR:-/tmp}/strace.$MAS_USE_RUN_STRACE.`datemt`.tmp"
  fi
#  export LD_DEBUG_OUTPUT="${MAS_PROJECTS_TMPDIR:-/tmp}/${bin_name}.ld_debug"
#  export LD_DEBUG='all'

  if [[ "$MAS_SOURCED" ]] ; then
    echo "MAS_SOURCED: $MAS_SOURCED" >&2
    cmd_exec="eval $straceit"
  else
#   cmd_exec="exec strace -e open  -o strace.tmp"
    cmd_exec="exec $straceit"
  fi
  if [[ "$made" ]] || make_m ; then
#   echo "......... $binary" >&2
    if cd $bin_dir && [[ "$bin_run" ]] ; then
      if type nanosleep >/dev/null 2>&1 ; then
        echo "1 To run rbinary: $cmd_exec $bin_run $@" >&2
        nanosleep 0.5 &&  $cmd_exec $bin_run "$@"
      elif type usleep >/dev/null 2>&1 ; then
        echo "2 To run rbinary: $cmd_exec $bin_run $@" >&2
        usleep 50000 &&  $cmd_exec $bin_run "$@"
      elif [[ -f "/usr/lib/libtcmalloc.so" ]] ; then
        echo "3 To run rbinary: $cmd_exec $bin_run $@" >&2
        $bin_run "$@"
      else
        echo "4 To run rbinary: $cmd_exec $bin_run $@" >&2
        $cmd_exec $bin_run "$@"
      fi
###   if type nanosleep >/dev/null 2>&1 ; then
###     echo "1 To run rbinary: $cmd_exec $bin_run $@" >&2
###     nanosleep 0.5 && LD_PRELOAD="/usr/lib/libtcmalloc.so"   $cmd_exec $bin_run "$@"
###   elif type usleep >/dev/null 2>&1 ; then
###     echo "2 To run rbinary: $cmd_exec $bin_run $@" >&2
###     usleep 50000 && LD_PRELOAD="/usr/lib/libtcmalloc.so"   $cmd_exec $bin_run "$@"
###   elif [[ -f "/usr/lib/libtcmalloc.so" ]] ; then
###     echo "3 To run rbinary: $cmd_exec $bin_run $@" >&2
###     LD_PRELOAD="/usr/lib/libtcmalloc.so" $cmd_exec $bin_run "$@"
###   else
###     echo "4 To run rbinary: $bin_run" >&2
###     $cmd_exec $bin_run "$@"
###   fi
    else
      echo "run error" >&2
    fi
  else
   echo "make error" >&2
  fi
  psshow
  echo "bash:$MAS_ZOCROMAS_HERE exited" >&2
}
function eddiffconfig ()
{
  if [[ "$rname_preset" ]] && [[ -f $runconfigdir/${rname_preset} ]] ; then
    gvim -d $runconfigdir/${rname_preset}{.$PPID,}
  elif [[ $mas_name =~ ^zocromas_(.*)$ ]] && rname_preset=${BASH_REMATCH[1]} && [[ "$rname_preset" ]] && [[ -f $runconfigdir/${rname_preset} ]] ; then
    gvim -d $runconfigdir/${rname_preset}{.$PPID,}
  else
    echo "mas_name: ${mas_name}" >&2
    echo "Pleasem set " >&2
    echo "  export MAS_ZOCROMAS_HERE=server" >&2
    echo "    or" >&2
    echo "  export MAS_ZOCROMAS_HERE=client" >&2
  fi
}
function psshowzz ()
{
  local names='zocDaeBunch,zocDaeBunchI,zocDaeBunchX,zocDaeMaster,zocDaeMasterTh,zocDaeMasterThX,zocDaeMasterX,zocDaemon,zocDaemonI,zocDaeServerD,zoclient,zocListen,zocListenIn,zocListenT,zocListenW,zocListenXit,zocLogger,zocLoggerXit,zocMain,zocMainAtexit,zocMainXit,zocParBunch,zocParBunchI,zocParBunchX,zocParent,zocParMaster,zocParMasterTh,zocParMasterThX,zocParMasterX,zocParServerD,zocTick,zocTickXit,zocTransTh,zocTransThXit,zocWatchLS,zocWatchLSXit,zocWatchTh,zocWatchThXit'
# bsdstart,tty,ni,user,ppid,pid,lwp,%cpu,%mem,stat,rss,vsz,s,sz,thcount,fname,cmd
  if [[ "$COLUMNS" ]] && [[ "$COLUMNS" -gt 0 ]] ; then
    /bin/ps --cols="$COLUMNS" --sort -pcpu,pid -C$names $@
  else
    /bin/ps --sort -pcpu,pid -C$names $@
  fi
}
function server_pid ()
{
  if [[ -f "$flavourdir/run/zocromas_server.pid" ]] ; then
    cat $flavourdir/run/zocromas_server.pid
  elif [[ -f "/run/zocromas_server/zocromas_server.pid" ]] ; then
    cat /run/zocromas_server/zocromas_server.pid
  else
    psshowzz -opid=
  fi
}
function psshowz ()
{
  local sp=`server_pid`
  if ! [[ "$sp" ]] || ! ps -Lp $sp ; then
    psshowzz -L $@
  fi
}
function pslist () 
{
  local sep=${1:-,}
  psshowz -ocomm= | tr '\n' "$sep" | sed -e "s/$sep$//"
}
function psshow ()
{
  local PS_FORMAT=pcpu,tt,start,user,group,ppid,sid,pid,lwp,stat,s,%cpu,%mem,vsz,sz,rss,nlwp,comm,cmd
# if [[ "$COLUMNS" ]] && [[ "$COLUMNS" -gt 0 ]] ; then
#   psshowz | cut -b-$COLUMNS
# else
#   psshowz
# ##    | cut -b-150 | sed -ne 's/$/  .../p'
# fi
  if psshowz >/dev/null && psshowz ; then
    return 0
  else
    echo "No" >&2
    return 1
  fi
}
function psshowc ()
{
  psshow | grep '\(No\| \<zoc\S\+\)'
}


