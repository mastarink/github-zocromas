function run_installed ()
{
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

  if [[ "$rname" ]] ; then
     echo "<<< $rname >>>" >&2
#      env | grep MAS_ZOCROMAS >&2

  # for core dump:
     ulimit -c unlimited

#     make_any && usleep 500000 && clear && exec $builddir/$rname "$@"
#      echo "bash:to run  $builddir/$rname" >&2
     echo "bash:starting $MAS_ZOCROMAS_HERE : $binary" >&2
     if type nanosleep >/dev/null 2>&1 ; then
       make_any && nanosleep 50000 && exec $binary "$@"
     elif type usleep >/dev/null 2>&1 ; then
       make_any && usleep 50000 && exec $binary "$@"
     else
       make_any && exec $binary "$@"
     fi
     echo "bash:$MAS_ZOCROMAS_HERE exited" >&2
  fi
}
function run_any ()
{
  echo "To RUN rbinary: $rbinary_preset" >&2
  # for core dump:
  ulimit -c unlimited

  #     make_any && usleep 500000 && clear && exec $builddir/$rname "$@"
  #      echo "bash:to run  $builddir/$rname" >&2
  if [[ "$made" ]] ||  make_any ; then
   if type nanosleep >/dev/null 2>&1 ; then
     nanosleep 50000 && LD_PRELOAD="/usr/lib/libtcmalloc.so"  exec $rbinary_preset "$@"
   elif type usleep >/dev/null 2>&1 ; then
     usleep 50000 && LD_PRELOAD="/usr/lib/libtcmalloc.so"  exec $rbinary_preset "$@"
   else
     LD_PRELOAD="/usr/lib/libtcmalloc.so"  exec $rbinary_preset "$@"
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
function psshow ()
{
  export PS_FORMAT=tt,start,user,ppid,sid,pid,lwp,stat,s,%cpu,%mem,vsz,sz,rss,nlwp,comm,cmd
# bsdstart,tty,ni,user,ppid,pid,lwp,%cpu,%mem,stat,rss,vsz,s,sz,thcount,fname,cmd
  /bin/ps ww  -L --sort -pcpu,pid -Czoclient,zocromas_server,zocbunch,zocchild,zocmaster | cut -b-150 | sed -ne 's/$/  .../p'
}
function server_pid ()
{
  local spid name
  for name in zocromas_server zocchild zocbunch ; do
    if spid=$( ps -C $name -o pid= ) ; then
      break
    fi
  done
  echo $spid
}


