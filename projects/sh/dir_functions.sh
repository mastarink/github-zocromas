function make_dirs ()
{
# if [[ -d "$projectsdir" ]] && ! [[ -d "$savedir" ]]; then
#   mkdir "$savedir" || echo "$LINENO ERROR make_dirs" >&2
# fi
  if ! [[ -d "$projectsdir" ]] ; then return 1 ; fi
  if [[ -d "$savedir" ]] && ! [[ -d "$savedirdist" ]]; then
    mkdir "$savedirdist" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$savedir" ]] && ! [[ -d "$savedirgentoo" ]]; then
    mkdir "$savedirgentoo" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$savedir" ]] && ! [[ -d "$savedirtar" ]]; then
    mkdir "$savedirtar" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$savedirtar" ]] && ! [[ -d "$savedirtarme" ]]; then
    mkdir "$savedirtarme" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$projectsdir" ]] && ! [[ -d "$instdir" ]]; then
    mkdir "$instdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$projectsdir" ]] && ! [[ -d "$tmpdir" ]]; then
    mkdir "$tmpdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$tmpdir" ]] && ! [[ -d "$tmpbuild" ]]; then
    mkdir "$tmpbuild" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$auxdir" ]] && ! [[ -d "$auxdir" ]]; then
    mkdir "$auxdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$m4dir" ]] && ! [[ -d "$m4dir" ]]; then
    mkdir "$m4dir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$tmpdir" ]] && ! [[ -d "$tmpunpack" ]]; then
    mkdir "$tmpunpack" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$debugdir" ]] && ! [[ -d "$debugdir" ]]; then
    mkdir "$debugdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$testdir" ]] && ! [[ -d "$testdir" ]]; then
    mkdir "$testdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$wbuilddir" ]] && ! [[ -d "$wbuilddir" ]]; then
    mkdir "$wbuilddir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$make_errdir" ]] && ! [[ -d "$make_errdir" ]]; then
    mkdir "$make_errdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
}
function setup_dirs ()
{
  if ! [[ "$stamp" ]] ; then
    if [[ "$MAS_DOALL_STAMP" ]] ; then
      stamp=$MAS_DOALL_STAMP
    else
      stamp=`/bin/date '+%Y%m%d'`
    fi
  fi
  if ! [[ "$now_stamp" ]] ; then
    if [[ "$MAS_DOALL_NOW_STAMP" ]] ; then
      now_stamp=$MAS_DOALL_NOW_STAMP
    else
      now_stamp=`/bin/date '+%Y%m%d.%H%M%S'`
    fi
  fi  
  wd=`pwd`
  me="$wd/$0"
  mer="$(realpath $0)" || return 1
  if [[ -f "$me" ]] && [[ -x "$me" ]] ; then
    shdir=$( dirname "$me" )
    shdirr=$( dirname "$mer" )
    shdirup=$( dirname $shdir )
    shdirupr=$( dirname $shdirr )
    indir="$( realpath "$shdirup" )" || return 1
    indirr="$( realpath "$shdirupr" )" || return 1
    indirrr="$( realpath "$shdirupr/.." )" || return 1
    updir="$( realpath $indir/.. )" || return 1
    updirr="$( realpath $indirr/.. )" || return 1
    projectsdir=$indirr
    admindir=$indirrr/admin
    runconfigdirname=.zocromas
    runconfigdir=$HOME/$runconfigdirname/
    runconfigdir=$projectsdir/zocromas/$runconfigdirname/
    export    MAS_WORK_ROOT_DIR=$projectsdir
    export    MAS_WORK_IN_DIR=$projectsdir

    savedir=$(realpath "$admindir/saved/" ) || return 1
    instdir=$(realpath "$admindir/install" ) || return 1
    instshdir=$(realpath "$admindir/install.sh" ) || return 1
    tmpdir=$(realpath "$admindir/tmp/" ) || return 1
    
    savedirdist=$(realpath "$savedir/dist/" ) || return 1
    savedirgentoo=$(realpath "$savedir/gentoo/" ) || return 1
    savedirtar=$(realpath "$savedir/tar/" ) || return 1
    savedirtarme=$(realpath "${savedirtar}/${stamp}" ) || return 1
    
    tmpbuild=$(realpath "$tmpdir/build/" ) || return 1
    tmpunpack=$(realpath "$tmpdir/unpack/" ) || return 1

    debugdir="$indir/debug"
    testdir="$indir/test"
    
    auxdir="$indir/.auxdir"
    m4dir="$auxdir/m4"
    inbuilddir="$auxdir/.build"
    wbuilddir="$inbuilddir"
    build_at="$wbuilddir"
    bsrcdir="$build_at/src"
    make_errdir="$inbuilddir/.errors"
    make_logname="makes.log"
    make_logfile="$make_errdir/$make_logname"
    logdir="$admindir/log"

    if [[ -d "$projectsdir" ]] ; then
      prjgroup=$(basename $projectsdir)
      make_dirs || return 1
  #   export LD_LIBRARY_PATH=/usr/local/lib

      export PKG_CONFIG_PATH=$instdir/lib/pkgconfig
    fi
    binprefix=zocromas_
  else
    echo "FATAL : '$me' bad SCRIPT call ($0)" >&2
  fi
  setup_vers || return 1
  return 0
}
