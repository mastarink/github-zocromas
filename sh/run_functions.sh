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
  local binary rbinary builddir bname rname mcaller made rprefix
  mcaller=$1
  shift
  rprefix=$binprefix
  if [[ "$mcaller" == '.' ]] ; then
    if [[ "$MAS_ZOCROMAS_HERE" ]] ; then
      mcaller=$MAS_ZOCROMAS_HERE
    elif make_any && [[ -x "$build_at/src/${rprefix}${prjname}" ]] ; then
      made=1
      mcaller=$prjname
    elif make_any && [[ -x "$build_at/src/${rprefix}${mas_name}" ]] ; then
      made=1
      mcaller=$mas_name
    elif make_any && [[ -x "$build_at/src/${mas_name}" ]] ; then
      made=1
      mcaller=$mas_name
      rprefix=''
    else
      echo "ERROR : prjname=$prjname ; no x: $build_at/src/[${rprefix}]${prjname}" >&2
      echo "ERROR : mas_name=$mas_name ; no x: $build_at/src/[${rprefix}]${mas_name}" >&2
    fi
  fi
#   echo "Please set " >&2
#   echo "  export MAS_ZOCROMAS_HERE=server" >&2
#   echo "    or" >&2
#   echo "  export MAS_ZOCROMAS_HERE=client" >&2
  rname=$( basename $mcaller )
  if [[ $rname =~ run_([a-z]+)\.sh ]] ; then
    bname=${BASH_REMATCH[1]}
    rname="${rprefix}$bname"
  elif [[ $rname =~ ^([a-z]+)$ ]] ; then
    bname=${BASH_REMATCH[1]}
    rname="${rprefix}$bname"
  else
    echo "$LINENO error ; rname:'$rname'" >&2
  fi

# show_setup
  if [[ -d "$build_at/src/$bname" ]] ; then
    builddir="$build_at/src/$bname"
  else
    builddir="$build_at/src"
  fi
  binary="$builddir/$rname"
  rbinary=$( realpath --relative-to=. $binary )
  echo "mcaller:$mcaller" >&2
  echo "rname:$rname" >&2
  echo "bname:$bname" >&2
  echo "instdir:$instdir" >&2
  echo "binary:$binary" >&2
  echo "rbinary:$rbinary" >&2
  echo "MAS_ZOCROMAS_HERE:$MAS_ZOCROMAS_HERE" >&2

  if [[ "$rname" ]] ; then
     echo "<<< $rname >>>" >&2
#      env | grep MAS_ZOCROMAS >&2

  # for core dump:
     ulimit -c unlimited

#     make_any && usleep 500000 && clear && exec $builddir/$rname "$@"
#      echo "bash:to run  $builddir/$rname" >&2
     echo "bash:starting $MAS_ZOCROMAS_HERE : $rbinary" >&2
     if [[ "$made" ]] ||  make_any ; then
       usleep 50000 && exec $rbinary "$@"
     else
       echo "make error" >&2
     fi
     echo "bash:$MAS_ZOCROMAS_HERE exited" >&2
  fi
}
function eddiffconfig ()
{
  local here
  if [[ "$MAS_ZOCROMAS_HERE" ]] ; then
    here=$MAS_ZOCROMAS_HERE
  elif [[ -x "$build_at/src/${rprefix}${prjname}" ]] ; then
    here=$prjname
  elif [[ -x "$build_at/src/${rprefix}${mas_name}" ]] ; then
    here=$mas_name
  elif [[ -x "$build_at/src/${mas_name}" ]] ; then
    here=$mas_name
  fi
  if [[ "$here" ]] && [[ -f $runconfigdir/${here} ]] ; then
    gvim -d $runconfigdir/${here}{.$PPID,}
  elif [[ $mas_name =~ ^zocromas_(.*)$ ]] && here=${BASH_REMATCH[1]} && [[ "$here" ]] && [[ -f $runconfigdir/${here} ]] ; then
    gvim -d $runconfigdir/${here}{.$PPID,}
  else
    echo "mas_name: ${mas_name}" >&2
    echo "Pleasem set " >&2
    echo "  export MAS_ZOCROMAS_HERE=server" >&2
    echo "    or" >&2
    echo "  export MAS_ZOCROMAS_HERE=client" >&2
  fi
}

