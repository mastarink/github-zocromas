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
     make_any && usleep 50000 && exec $binary "$@"
     echo "bash:$MAS_ZOCROMAS_HERE exited" >&2
  fi
}
function run_any ()
{
  local binary builddir bname rname mcaller made
  mcaller=$1
  shift
  if [[ "$mcaller" == '.' ]] ; then
    if [[ "$MAS_ZOCROMAS_HERE" ]] ; then
      mcaller=$MAS_ZOCROMAS_HERE
    elif make_any && [[ -x "$build_at/src/${binprefix}${prjname}" ]] ; then
      made=1
      mcaller=$prjname
    else
      echo "ERROR : prjname=$prjname ; no x: $build_at/src/${binprefix}${prjname}" >&2
    fi
  fi
#   echo "Please set " >&2
#   echo "  export MAS_ZOCROMAS_HERE=server" >&2
#   echo "    or" >&2
#   echo "  export MAS_ZOCROMAS_HERE=client" >&2
  rname=$( basename $mcaller )
  if [[ $rname =~ run_([a-z]+)\.sh ]] ; then
    bname=${BASH_REMATCH[1]}
    rname="${binprefix}$bname"
  elif [[ $rname =~ ^([a-z]+)$ ]] ; then
    bname=${BASH_REMATCH[1]}
    rname="${binprefix}$bname"
  fi

# show_setup
  if [[ -d "$build_at/src/$bname" ]] ; then
    builddir="$build_at/src/$bname"
  else
    builddir="$build_at/src"
  fi
  binary="$builddir/$rname"

  if [[ "$rname" ]] ; then
     echo "<<< $rname >>>" >&2
#      env | grep MAS_ZOCROMAS >&2

  # for core dump:
     ulimit -c unlimited

#     make_any && usleep 500000 && clear && exec $builddir/$rname "$@"
#      echo "bash:to run  $builddir/$rname" >&2
     echo "bash:starting $MAS_ZOCROMAS_HERE : $binary" >&2
     if [[ "$made" ]] ||  make_any ; then
       usleep 50000 && exec $binary "$@"
     else
       echo "make error" >&2
     fi
     echo "bash:$MAS_ZOCROMAS_HERE exited" >&2
  fi
}
