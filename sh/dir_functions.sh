function make_dirs ()
{
  if [[ -d "$rootdir" ]] && ! [[ -d "$savedir" ]]; then
    mkdir "$savedir"
  fi
  if [[ -d "$savedir" ]] && ! [[ -d "$savedirdist" ]]; then
    mkdir "$savedirdist"
  fi
  if [[ -d "$savedir" ]] && ! [[ -d "$savedirtar" ]]; then
    mkdir "$savedirtar"
  fi
  if [[ -d "$savedirtar" ]] && ! [[ -d "$savedirtarme" ]]; then
    mkdir "$savedirtarme"
  fi
  if [[ -d "$rootdir" ]] && ! [[ -d "$instdir" ]]; then
    mkdir "$instdir"
  fi
  if [[ -d "$rootdir" ]] && ! [[ -d "$tmpdir" ]]; then
    mkdir "$tmpdir"
  fi
  if [[ -d "$tmpdir" ]] && ! [[ -d "$tmpbuild" ]]; then
    mkdir "$tmpbuild"
  fi
  if [[ -d "$tmpdir" ]] && ! [[ -d "$tmpunpack" ]]; then
    mkdir "$tmpunpack"
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
    if [[ -d "$rootdir" ]] ; then
      prjgroup=$(basename $rootdir)
      make_dirs
  #   export LD_LIBRARY_PATH=/usr/local/lib

      prjname=$( basename $indir )
      setup_vers
      export PKG_CONFIG_PATH=$instdir/lib/pkgconfig
    fi
  else
    echo "FATAL : bad script call ($0)" >&2
  fi
}

