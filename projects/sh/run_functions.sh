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
  echo "To RUN rbinary: $rbinary_preset" >&2
  # for core dump:
  ulimit -c unlimited
  
  #     make_any && usleep 500000 && clear && $cmd_exec $builddir/$rname "$@"
  #      echo "bash:to run  $builddir/$rname" >&2

  if false ; then
    straceit='strace -fr -C -o strace.tmp'
  fi


  if [[ "$MAS_SOURCED" ]] ; then
    echo "MAS_SOURCED: $MAS_SOURCED" >&2
    cmd_exec="eval $straceit"
  else
#   cmd_exec="exec strace -e open  -o strace.tmp"
    cmd_exec="exec $straceit"
  fi
  if [[ "$made" ]] ||  make_any ; then
    if [[ "$rbinary_preset" ]] ; then
      if type nanosleep >/dev/null 2>&1 ; then
        echo "1 To run rbinary: $cmd_exec $rbinary_preset $@" >&2
        nanosleep 0.5 && LD_PRELOAD="/usr/lib/libtcmalloc.so"   $cmd_exec $rbinary_preset "$@"
      elif type usleep >/dev/null 2>&1 ; then
        echo "2 To run rbinary: $cmd_exec $rbinary_preset $@" >&2
        usleep 50000 && LD_PRELOAD="/usr/lib/libtcmalloc.so"   $cmd_exec $rbinary_preset "$@"
      elif [[ -f "/usr/lib/libtcmalloc.so" ]] ; then
        echo "3 To run rbinary: $cmd_exec $rbinary_preset $@" >&2
        LD_PRELOAD="/usr/lib/libtcmalloc.so" $cmd_exec $rbinary_preset "$@"
      else
        echo "4 To run rbinary: $rbinary_preset" >&2
        $cmd_exec $rbinary_preset "$@"
      fi
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
function psshow ()
{
  export PS_FORMAT=tt,start,user,ppid,sid,pid,lwp,stat,s,%cpu,%mem,vsz,sz,rss,nlwp,comm,cmd
# bsdstart,tty,ni,user,ppid,pid,lwp,%cpu,%mem,stat,rss,vsz,s,sz,thcount,fname,cmd
  if [[ "$COLUMNS" ]] && [[ "$COLUMNS" -gt 0 ]] ; then
    /bin/ps ww  -L --sort -pcpu,pid -Czoclient,zocromas_server,zocbunch,zocchild,zocmaster | cut -b-$COLUMNS
  else
    /bin/ps ww  -L --sort -pcpu,pid -Czoclient,zocromas_server,zocbunch,zocchild,zocmaster | cut -b-150 | sed -ne 's/$/  .../p'
  fi
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


