# unalias cp
. sh/dir_functions.sh
. sh/make_functions.sh
function prjconfV ()
{
  if [[ -f "./configure" ]] ; then
    ./configure -V
  elif [[ -f "./config.status" ]] ; then
    ./config.status -V
  elif [[ "$build_at" ]] && [[ -f "$build_at/config.status" ]] ; then
    $build_at/config.status -V
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
  local n v rprefix rname_case
  n=$1
  shift
  v=$1
  shift
  if ! [[ "$n" ]] ; then n=`prjconfname`    ; fi
  if ! [[ "$v" ]] ; then v=`prjconfversion` ; fi
# echo "`pwd` SV>> $n . $v" >&2
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
#     echo "`pwd`>> [$mas_name] [$mas_vers]" >&2
      mas_base_vers='0.0.5.20130219'
    fi
    configure_opts="--prefix=$instdir --silent --enable-silent-rules --enable-tracemem --enable-debug"
    configure_opts="$configure_opts --with-base-dir=/mnt/new_misc/develop/autotools/zoc"
    configure_opts="$configure_opts --with-pids-dir=zocromas/pid"
    configure_opts="$configure_opts --with-mods-dir=zocmds"
    configure_opts="$configure_opts --with-proto-dir=zocromas_zocprotos"
    configure_opts="$configure_opts --with-log-dir=log"
    configure_opts="$configure_opts --with-server=/tmp/zocromas.socket"
    configure_opts="$configure_opts --with-def-proto=xcromas"
    instshname="$instshdir/${mas_name}-${mas_vers}.sh"
    mas_fullname="${mas_name}-${mas_vers}"
  fi
# echo "[$mas_name] [$mas_vers]" >&2
  prjname=$( basename $indir )
# ebuild_prefix=zocromas_
  ebuild_dir=$rootdir/ebuilds/mas-tar/${ebuild_prefix}${mas_name}

  rprefix=$binprefix
 
  mcaller_preset=$0
  mcaller_fname=$( basename $0 )
  if [[ $mcaller_fname =~ (run|debug|gdbcore)_(${rprefix}.*)\.sh$ ]] ; then
    rname_preset=${BASH_REMATCH[2]}
    rname_case=1
  elif [[ "$MAS_ZOCROMAS_HERE" ]] ; then
    rname_preset=$MAS_ZOCROMAS_HERE
    rname_case=2
  elif binary_preset="$build_at/src/${rprefix}${prjname}"  && [[ -x $binary_preset ]] ; then
    rname_preset=$prjname
    rname_case=3
  elif binary_preset="$build_at/src/${rprefix}${mas_name}" && [[ -x $binary_preset ]] ; then
    rname_preset=$mas_name
    rname_case=4
  elif binary_preset="$build_at/src/${mas_name}"           && [[ -x $binary_preset ]] ; then
    rname_preset=$mas_name
    rname_case=5
    rprefix=''
# else
#   echo "ERROR : prjname=$prjname ; no x: $build_at/src/[${rprefix}]${prjname}" >&2
#   echo "ERROR : mas_name=$mas_name ; no x: $build_at/src/[${rprefix}]${mas_name}" >&2
  fi
  if [[ "${binprefix}" ]] && [[ "$rname_preset" =~ ^${binprefix}(.+)$ ]] ; then
    short_name=${BASH_REMATCH[1]}
  else
    echo "$rname_preset ::: ${binprefix} :: ---" >&2
  fi
  if [[ "$binary_preset" ]] && [[ -f "$binary_preset" ]] ; then
    rbinary_preset=$( realpath --relative-to=$indir $binary_preset )
  fi
# rname_preset=$( basename $mcaller )
# if [[ $rname =~ run_([a-z]+)\.sh ]] ; then
#   bname=${BASH_REMATCH[1]}
#   rname="${rprefix}$bname"
# elif [[ $rname =~ ^([a-z]+)$ ]] ; then
#   bname=${BASH_REMATCH[1]}
#   rname="${rprefix}$bname"
# else
#   echo "$LINENO error ; rname:'$rname'" >&2
# fi

# echo ">>>>>>> setup_vers <<<<<<" >&2
# echo "@<<binprefix [$binprefix] >>@" >&2
# echo "@<<rprefix [$rprefix] >>@" >&2
# echo "@<<mcaller_preset $mcaller_preset >>@" >&2
# echo "@<<mcaller_fname $mcaller_fname >>@" >&2
# echo "@<<rname_preset [$rname_case]  $rname_preset >>@" >&2
# echo "@<<short_name $short_name >>@" >&2
# echo "@<<binary_preset $binary_preset >>@" >&2
# echo "@<<rbinary_preset $rbinary_preset >>@" >&2
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
# echo "SETUPZ" >&2
setup_dirs
# echo "/SETUPZ" >&2
