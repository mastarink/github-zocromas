#!/bin/sh

function prjconfname ()
{
  if [[ -f "./config.status" ]] ; then
    ./config.status -V|head -1|awk '{print $1}'
  elif [[ -f "./configure" ]] ; then
    ./configure -V|head -1|awk '{print $1}'
  fi
}
function prjconfversion ()
{
  if [[ -f "./config.status" ]] ; then
    ./config.status -V|head -1|awk '{print $3}'
  elif [[ -f "./configure" ]] ; then
    ./configure -V|head -1|awk '{print $3}'
  fi
}  
function setup_dirs ()
{
  rootdir=$(dirname $(dirname $(realpath $0)))
  prjgroup=$(basename $rootdir)
  if [[ -d "$rootdir" ]] ; then
    stamp=`datem`
    savedir=$( realpath "$rootdir/saved/" )
    if [[ -d "$rootdir" ]] && ! [[ -d "$savedir" ]]; then
      mkdir "$savedir"
    fi
    savedirdist="$savedir/dist/"
    if [[ -d "$savedir" ]] && ! [[ -d "$savedirdist" ]]; then
      mkdir "$savedirdist"
    fi
    savedirtar="$savedir/tar/"
    if [[ -d "$savedir" ]] && ! [[ -d "$savedirtar" ]]; then
      mkdir "$savedirtar"
    fi
    savedirtarme="${savedirtar}/${stamp}"
    if [[ -d "$savedirtar" ]] && ! [[ -d "$savedirtarme" ]]; then
      mkdir "$savedirtarme"
    fi
    instdir="$rootdir/install"
    if [[ -d "$rootdir" ]] && ! [[ -d "$instdir" ]]; then
      mkdir "$instdir"
    fi
#     export LD_LIBRARY_PATH=/usr/local/lib
    tmpdir="$rootdir/tmp/"
    if [[ -d "$rootdir" ]] && ! [[ -d "$tmpdir" ]]; then
      mkdir "$tmpdir"
    fi
    wd=`pwd`
    me="$wd/$0"
    shdir=$( dirname "$me" )
    shdirup=$( dirname $shdir )
    indir="$( realpath "$shdirup" )"

    updir="$( realpath $indir/.. )"
    debugdir=$indir/debug
    prjname=$( basename $indir )
    name=`prjconfname`
    ver=`prjconfversion`
    workdir=$tmpdir
    echo "name:$name; ver:$ver" >&2
    if [[ "$name" ]] && [[ "$ver" ]] ; then
      unpackdir="$workdir/${name}-${ver}"
      ibuilddir=$workdir/build/$name
    fi
    wbuilddir=$indir
    if ! [[ "$configuredir" ]] ; then
      if [[ -f "$indir/configure" ]] ; then
	configuredir=$indir
      elif [[ -f "$unpackdir/configure" ]] ; then
	configuredir=$unpackdir
      fi
    fi
  fi
  export PKG_CONFIG_PATH=$instdir/lib/pkgconfig
}
function show_setup ()
{
  echo "wd: $wd" >&2
  echo "0: $0" >&2
  echo "me: $me" >&2
  echo "shdir: $shdir" >&2
  echo "shdirup: $shdirup" >&2
  echo "indir: $indir" >&2
  echo "updir: $updir" >&2
  echo "savedir: $savedir" >&2
  echo "name: $name" >&2
  echo "ver: $ver" >&2
}
function make_target ()
{
  local ilog target
  target=$1
  shift
# echo "install [$prjname]" >&2
  ilog="/tmp/$target-$prjname.log"
  if make -s $target > $ilog ; then
    echo "$prjname $target OK : $ilog" >&2
  else
    echo "$prjname $target FAIL" >&2
    cat "$ilog" >&2
    return 1
  fi
  return 0
}
function run_any ()
{
  local mcaller=$1
  shift
  local rname=$( basename $mcaller )
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

  if [[ "$rname" ]] ; then
     echo "<<< $rname >>>" >&2
#      env | grep MAS_ZOCROMAS >&2

  # for core dump:
     ulimit -c unlimited

#     make_any && usleep 500000 && clear && exec $srcdir/$rname "$@"
#      echo "bash:to run  $srcdir/$rname" >&2
     echo "bash:starting $MAS_ZOCROMAS_HERE" >&2
     make_any && usleep 50000 && exec $srcdir/$rname "$@"
     echo "bash:$MAS_ZOCROMAS_HERE exited" >&2
  fi
}
function mas_debug ()
{
  local sedex cmdfile tmpcmd lt

  local mcaller=$1
  shift
  local rname=$( basename $mcaller )
  if [[ $rname =~ debug_([a-z]+)\.sh ]] ; then
    bname=${BASH_REMATCH[1]}
    rname="mas_$bname"
  elif [[ $rname =~ ^([a-z]+)$ ]] ; then
    bname=${BASH_REMATCH[1]}
    rname="mas_$bname"
  fi

  if [[ -d "$indir/src/$bname" ]] ; then
    srcdir="$indir/src/$bname"
  else
    srcdir="$indir/src"
  fi

  if [[ -f "$srcdir/.libs/$rname" ]] ; then
    binsdir="$srcdir/.libs"
    lt=yes
  else
    binsdir="$srcdir"
    unset lt
  fi
  libsdirs="$( find -type d -name '.libs' -printf '%p:' )"

echo "name: $name" >&2
echo "bname: $bname" >&2
echo "rname: $rname" >&2
echo "srcdir: $srcdir" >&2
echo "binsdir: $binsdir" >&2


  export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$libsdirs"
  ulimit -c

  cmdfile="$debugdir/debug_${rname}.cmd"
  sedex="s@^\(run\)@\1  $@@"

  tmpcmd="${cmdfile}.tmp"
  sed -e "$sedex" "$cmdfile" > $tmpcmd
  echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH" >&2
  

  if [[ "$lt" ]] ; then
    echo "[$mcaller] [$rname] [$bname] {$cmdfile} $binsdir/$rname -- $tmpcmd" >&2
    if [[ -f "$tmpcmd" ]] && [[ -s "$tmpcmd" ]] ; then
  # libtool --mode=execute gdb -batch $binsdir/$name -x $tmpcmd
      libtool --mode=execute gdb        $binsdir/$rname -x $tmpcmd
    else
      echo "no file : $cmdfile" >&2
    fi
  else
    echo "[$mcaller] [$rname] [$bname] {$cmdfile} $binsdir/$rname -- $tmpcmd" >&2
    gdb        $binsdir/$rname -x $tmpcmd
  fi
}
function gdb_core_any ()
{
  mcaller=$1
  shift
  name=$( basename $mcaller )
  if [[ $name =~ gdbcore_([a-z]+)\.sh ]] ; then
    bname=${BASH_REMATCH[1]}
    name="mas_$bname"
  fi
  echo "<< $name >>">&2

  srcdir0="$indir/src"
  srcdir="$indir/src/$bname"
  binsdir="$srcdir/.libs"
  # libsdirs="$srcdir0/.libs"
  libsdirs="$( find -type d -name '.libs' -printf '%p ' )"
  #for dl in server fileinfo init tools cmdmods maslibs client http listener channel ; do
  # libsdirs="$libsdirs:$srcdir0/$dl/.libs"
  #done


  export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$libsdirs"
  ulimit -c

  corename=$( ls -1tr /tmp/core.$UID.$UID.${name}.* | tail -1 )
  echo "name: {$name} ; core:{$corename}" >&2
  if [[ -f "$corename" ]] ; then
    libtool --mode=execute gdb $binsdir/$name -c "$corename"
  fi
}
function debug_any ()
{
#   clear && 
  make_any && mas_debug $@
}
function masmake ()
{
  local ername erdir
  erdir='errors'
  ername="$erdir/make.`datemt`.result"
  if [[ "$erdir" ]] && ! [[ -d "$erdir" ]] ; then
    mkdir $erdir
  fi
  if [[ "$ername" ]] && ! [[ -f "$ername" ]] ; then
    if make -s $@ >$ername 2>&1 ; then
      rm $ername
      return 0
    elif [[ -s "$ername" ]] ; then
      echo -n "$ername >>>>>>>>" >&2
      if false ; then
	cat $ername
      else
	grep -v '^Making all in ' $ername >&2
      fi
      echo -n "<<<<<<<<" >&2
    # grep error $ername | head -1 | sed -ne 's/^\([a-z]\+\.[ch]\):\([[:digit:]]\+\):.*$/\1 :: \2/p'
    else
      echo "$LINENO ERROR make" >&2
    fi
  fi
  return 1
}
function gmasmake ()
{
# echo "gmasmake" >&2
# grep  --include='*.[ch]' -r '\<\(pipe\|fork\|waitpid\|accept\|connect\|bind\|getsockopt\|setsockopt\|inet_ntop\|listen\|close\|mkfifo\)\>\s*(' src inc | grep -v '^src/mas_lib[0-9]*\.c'
  if [[ -f "grepfunexclude.txt" ]] ; then
  ! grep -r --color --include='*.[ch]' --exclude-from="grepfunexclude.txt" -r '\<\(pipe\|fork\|waitpid\|accept\|connect\|bind\|getsockopt\|setsockopt\|inet_ntop\|listen\|close\|mkfifo\)\>\s*(' src && \
  ! grep -r --color --include='*.[ch]' --exclude-from="grepfunexclude.txt" -r '\<\(malloc\|free\|strdup\|strndup\)\>\s*(' src && \
    masmake
  else
    masmake
  fi
# make -s ctags
}
function make_any ()
{
# echo "make_any" >&2
  if [[ "$indir" ]] && [[ -d "$indir" ]] ; then
    cd $indir
    time gmasmake
  else
    echo "$LINENO ERROR : no dir $indir" >&2
#    exit
  fi
}
function doprj ()
{
  local prj act
  local list
  
  if [[ "$1" == 'full' ]] ; then
    list='autoreconf configure make install'
  else
    list=$@
  fi
  prj=$(basename `pwd`)
  if [[ -d sh ]] ; then
    for act in $list ; do
      shfile="sh/${act}.sh"
      if [[ "$act" ]] && [[ -L "sh" ]] && [[ -x $shfile ]] ; then
	echo "${nn}.	$act $prj" >&2
      # echo "$act at $dir" >&2
	if false ; then
	  echo "SKIP $shfile @ `pwd`" >&2
	elif ! eval $shfile ; then
	  echo "FAIL $act $prj" >&2
	  return 1
	fi
      else
	echo "${nn}.	>>>> skipping act [$act]" >&2
	return 1
      fi
    done
  fi
}
function doall ()
{  
  local prj nn
  if [[ -d "$tmpdir" ]] && [[ "$rootdir" ]] && [[ -d "$rootdir" ]] && cd $rootdir ; then
    if [[ -f projects.list ]] ; then
      list=`cat projects.list`
#	echo "pwd: {$( pwd )}" >&2
    else
      echo "not exists projects.list" >&2
      return 1
    fi
    nn=0
    for prj in $list ; do
      cd $rootdir
      dir=$( realpath $prj )  || return 1
      if [[ "$prj" ]] && [[ -d "$prj" ]] ; then
	cd $dir                 || return 1
	doprj $@ || return 1
      else
	echo "${nn}.	>>>> skipping dir [$dir]" >&2
      fi
      nn=$(( $nn + 1 ))
    done
  else
    return 1
  fi
  return 0
}
function configure ()
{
  if ! [[ "$wbuilddir" ]] ; then
    wbuilddir="$indir"
  fi
  if ! [[ "$configuredir" ]] ; then
    configuredir="$indir"
  fi
  if [[ "$wbuilddir" ]] && [[ -d "$wbuilddir" ]] ; then
    cd $wbuilddir
    echo "configure: wbuilddir=$wbuilddir; configuredir=$configuredir;" >&2
    if [[ -d "$instdir" ]] ; then
      echo "pwd:`pwd`; configuredir:$configuredir" >&2
      $configuredir/configure --prefix="$instdir" --silent --enable-silent-rules --enable-tracemem --enable-debug || return 1
    else
  #   ./configure --silent --enable-silent-rules --enable-tracemem --enable-debug
      echo "no dir $instdir" >&2
      return 1
    fi
  fi
  return 0
}

setup_dirs
