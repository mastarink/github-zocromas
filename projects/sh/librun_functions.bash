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

  binary="$instdir/bin/$rname"

# echo "mcaller:$mcaller" >&2
# echo "rname:$rname" >&2
# echo "bname:$bname" >&2
# echo "instdir:$instdir" >&2
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
  local cmd_exec
# echo "To RUN rbinary: $rbinary_preset" >&2
  # for core dump:
  ulimit -c unlimited
  
  #     make_any && usleep 500000 && clear && $cmd_exec $builddir/$rname "$@"
  #      echo "bash:to run  $builddir/$rname" >&2

  if true ; then
    straceit='strace -q -fr -C -o strace.tmp'
  fi


  if [[ "$MAS_SOURCED" ]] ; then
    echo "MAS_SOURCED: $MAS_SOURCED" >&2
    cmd_exec="eval $straceit"
  else
#   cmd_exec="exec strace -e open  -o strace.tmp"
    cmd_exec="exec $straceit"
  fi
  if [[ "$made" ]] ||  make_any ; then
#   echo "......... $rbinary_preset" >&2
    if [[ "$rbinary_preset" ]] ; then
      if type nanosleep >/dev/null 2>&1 ; then
#        echo "1 To run rbinary: $cmd_exec $rbinary_preset $@" >&2
        nanosleep 0.5 &&  $cmd_exec $rbinary_preset "$@"
      elif type usleep >/dev/null 2>&1 ; then
#        echo "2 To run rbinary: $cmd_exec $rbinary_preset $@" >&2
        usleep 50000 &&  $cmd_exec $rbinary_preset "$@"
      elif [[ -f "/usr/lib/libtcmalloc.so" ]] ; then
#        echo "3 To run rbinary: $cmd_exec $rbinary_preset $@" >&2
        $rbinary_preset "$@"
      else
#        echo "4 To run rbinary: $rbinary_preset" >&2
        $cmd_exec $rbinary_preset "$@"
      fi
###   if type nanosleep >/dev/null 2>&1 ; then
###     echo "1 To run rbinary: $cmd_exec $rbinary_preset $@" >&2
###     nanosleep 0.5 && LD_PRELOAD="/usr/lib/libtcmalloc.so"   $cmd_exec $rbinary_preset "$@"
###   elif type usleep >/dev/null 2>&1 ; then
###     echo "2 To run rbinary: $cmd_exec $rbinary_preset $@" >&2
###     usleep 50000 && LD_PRELOAD="/usr/lib/libtcmalloc.so"   $cmd_exec $rbinary_preset "$@"
###   elif [[ -f "/usr/lib/libtcmalloc.so" ]] ; then
###     echo "3 To run rbinary: $cmd_exec $rbinary_preset $@" >&2
###     LD_PRELOAD="/usr/lib/libtcmalloc.so" $cmd_exec $rbinary_preset "$@"
###   else
###     echo "4 To run rbinary: $rbinary_preset" >&2
###     $cmd_exec $rbinary_preset "$@"
###   fi
    else
      echo "run error" >&2
    fi
  else
   echo "make error" >&2
  fi
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
function psshowz ()
{
  psshowzz -L $@
}
function pslist () 
{
  local sep=${1:-,}
  psshowz -ocomm= | tr '\n' "$sep" | sed -e "s/$sep$//"
}
function psshow ()
{
  local PS_FORMAT=pcpu,tt,start,user,ppid,sid,pid,lwp,stat,s,%cpu,%mem,vsz,sz,rss,nlwp,comm,cmd
# if [[ "$COLUMNS" ]] && [[ "$COLUMNS" -gt 0 ]] ; then
#   psshowz | cut -b-$COLUMNS
# else
#   psshowz
# ##    | cut -b-150 | sed -ne 's/$/  .../p'
# fi
  psshowz >/dev/null && psshowz || echo "No" >&2
}
function psshowc ()
{
  psshow | grep '\(No\| \<zoc\S\+\)'
}
function server_pid ()
{
  psshowzz -opid=
}


