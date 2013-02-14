function run_any ()
{
  local binary srcdir bname rname mcaller
  mcaller=$1
  shift
  rname=$( basename $mcaller )
  if [[ $rname =~ run_([a-z]+)\.sh ]] ; then
    bname=${BASH_REMATCH[1]}
    rname="mas_$bname"
  elif [[ $rname =~ ^([a-z]+)$ ]] ; then
    bname=${BASH_REMATCH[1]}
    rname="mas_$bname"
  fi

# show_setup
  if [[ -d "$indir/src/$bname" ]] ; then
    srcdir="$indir/src/$bname"
  else
    srcdir="$indir/src"
  fi
  binary="$srcdir/$rname"

  if [[ "$rname" ]] ; then
     echo "<<< $rname >>>" >&2
#      env | grep MAS_ZOCROMAS >&2

  # for core dump:
     ulimit -c unlimited

#     make_any && usleep 500000 && clear && exec $srcdir/$rname "$@"
#      echo "bash:to run  $srcdir/$rname" >&2
     echo "bash:starting $MAS_ZOCROMAS_HERE : $binary" >&2
     make_any && usleep 50000 && exec $binary "$@"
     echo "bash:$MAS_ZOCROMAS_HERE exited" >&2
  fi
}
