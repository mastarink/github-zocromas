. sh/dir_functions.sh
. sh/make_functions.sh
function prjconfV ()
{
  if [[ "$build_at" ]] && [[ -f "$build_at/config.status" ]] ; then
    $build_at/config.status -V
  elif [[ -f "./config.status" ]] ; then
    ./config.status -V
  elif [[ -f "./configure" ]] ; then
    ./configure -V
  fi
}
function prjconffullversion ()
{
  prjconfV | head -1
}
function prjconfname ()
{
  prjconffullversion |awk '{print $1}'
}
function prjconfversion ()
{
  prjconffullversion |awk '{print $3}'
}  
function setup_vers ()
{
  local n v
  n=$1
  shift
  v=$1
  shift
  if ! [[ "$n" ]] ; then n=`prjconfname`    ; fi
  if ! [[ "$v" ]] ; then v=`prjconfversion` ; fi
  if [[ "$n" ]] && [[ "$v" ]] ; then
#   echo "[$( basename $0 )] SET name:$n; version:$v" >&2
    if [[ -f "$indir/configure" ]] ; then
      if [[ "$n" ]] && [[ "$v" ]] && [[ "$tmpunpack" ]] && [[ -d "$tmpunpack" ]] && [[ "$tmpbuild" ]] && [[ -d "$tmpbuild" ]] \
      			&& [[ "$savedirdist" ]] && [[ -d "$savedirdist" ]] ; then
	unpackdir="$tmpunpack/${n}-${v}"
	ibuilddir="$tmpbuild/${n}"
	distname="${n}-${v}.tar.bz2"
	distfile="$savedirdist/$distname"
	distfile_prefix="$savedirdist/${n}"
      fi
      wbuilddir="$inbuilddir"
      build_at="$wbuilddir"
      configuredir=$indir
      mas_name=$n
      mas_vers=$v
    fi
    configure_opts="--prefix=$instdir --silent --enable-silent-rules --enable-tracemem --enable-debug"
    instshname="$instshdir/${mas_name}-${mas_vers}.sh"
    mas_fullname="${mas_name}-${mas_vers}"
  fi
}
function show_setup ()
{
  echo "wd:		$wd" >&2
  echo "0:		$0" >&2
  echo "me:		$me" >&2
  echo "mer:		$mer" >&2
  echo "shdir:		$shdir" >&2
  echo "shdirup:	$shdirup" >&2
  echo "indir:		$indir" >&2
  echo "updir:		$updir" >&2
  echo "configuredir:	$configuredir" >&2
  echo "wbuilddir:	$wbuilddir" >&2
  
  echo "= SAVE:" >&2
  echo "savedir:	$savedir" >&2
  echo "savedirdist:	$savedirdist" >&2
  echo "savedirtar:	$savedirtar" >&2
  echo "savedirtarme:	$savedirtarme" >&2
  
  echo "= TMP:" >&2
  echo "tmpdir:		$tmpdir" >&2
  echo "tmpunpack:	$tmpunpack" >&2
  echo "tmpbuild:	$tmpbuild" >&2
  
  echo "= INSTALL:" >&2
  echo "instdir:	$instdir" >&2
  echo "unpackdir:	$unpackdir" >&2
  echo "ibuilddir:	$ibuilddir" >&2
  echo "distname:	$distname" >&2
  echo "distfile:	$distfile" >&2

  echo "= NAME / VER:" >&2
  echo "mas_name:	$mas_name" >&2
  echo "mas_vers:	$mas_vers" >&2

  echo "= ROOT:" >&2
  echo "rootdir:	$rootdir" >&2
  
}

export MAS_MAKE_CNT=0
setup_dirs
