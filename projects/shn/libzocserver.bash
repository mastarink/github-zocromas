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
function abrt_server ()
{
  local spid sig=$1
  shift
  if ! [[ "$sig" ]] ; then sig=QUIT ; fi
  if spid=$( server_pid ) && [[ "$spid" ]] && [[ "$sig" ]] ; then
    echo "[[ killing $spid with $sig ]]" >&2
    kill -$sig $spid && echo "KILLED $spid with $sig" >&2 || echo "NOT killed $spid with $sig"
  else
    echo "No server pid" >&2
  fi
  sleep 0.5
  psshow
}
