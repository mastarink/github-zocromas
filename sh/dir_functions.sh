function make_dirs ()
{
  if [[ -d "$rootdir" ]] && ! [[ -d "$savedir" ]]; then
    mkdir "$savedir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$savedir" ]] && ! [[ -d "$savedirdist" ]]; then
    mkdir "$savedirdist" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$savedir" ]] && ! [[ -d "$savedirtar" ]]; then
    mkdir "$savedirtar" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$savedirtar" ]] && ! [[ -d "$savedirtarme" ]]; then
    mkdir "$savedirtarme" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$rootdir" ]] && ! [[ -d "$instdir" ]]; then
    mkdir "$instdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$rootdir" ]] && ! [[ -d "$tmpdir" ]]; then
    mkdir "$tmpdir" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$tmpdir" ]] && ! [[ -d "$tmpbuild" ]]; then
    mkdir "$tmpbuild" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ -d "$tmpdir" ]] && ! [[ -d "$tmpunpack" ]]; then
    mkdir "$tmpunpack" || echo "$LINENO ERROR make_dirs" >&2
  fi
  if [[ "$wbuilddir" ]] && ! [[ -d "$wbuilddir" ]]; then
    mkdir "$wbuilddir" || echo "$LINENO ERROR make_dirs" >&2
  fi
}
function setup_dirs ()
{
  stamp=`datem`
  wd=`pwd`
  me="$wd/$0"
  mer="$(realpath $0)"
  if [[ -f "$me" ]] && [[ -x "$me" ]] ; then
    shdir=$( dirname "$me" )
    shdirr=$( dirname "$mer" )
    shdirup=$( dirname $shdir )
    shdirupr=$( dirname $shdirr )
    indir="$( realpath "$shdirup" )"
    indirr="$( realpath "$shdirupr" )"
    updir="$( realpath $indir/.. )"
    updirr="$( realpath $indirr/.. )"
    rootdir=$indirr
    export    MAS_WORK_ROOT_DIR=$rootdir
    export    MAS_WORK_IN_DIR=$rootdir

    savedir=$(realpath "$rootdir/saved/" )
    instdir=$(realpath "$rootdir/install" )
    instshdir=$(realpath "$rootdir/install.sh" )
    tmpdir=$(realpath "$rootdir/tmp/" )
    
    savedirdist=$(realpath "$savedir/dist/" )
    savedirtar=$(realpath "$savedir/tar/" )
    savedirtarme=$(realpath "${savedirtar}/${stamp}" )
    
    tmpbuild=$(realpath "$tmpdir/build/" )
    tmpunpack=$(realpath "$tmpdir/unpack/" )

    debugdir="$indir/debug"
    
    inbuilddir="$indir/.build"
    wbuilddir="$inbuilddir"
    build_at="$wbuilddir"

    if [[ -d "$rootdir" ]] ; then
      prjgroup=$(basename $rootdir)
      make_dirs
  #   export LD_LIBRARY_PATH=/usr/local/lib

      prjname=$( basename $indir )
      ebuild_dir=$rootdir/ebuilds/mas-tar/$mas_name
      setup_vers
      export PKG_CONFIG_PATH=$instdir/lib/pkgconfig
    fi
    binprefix=mas_
  else
    echo "FATAL : bad script call ($0)" >&2
  fi
}

