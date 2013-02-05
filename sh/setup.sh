#!/bin/sh
if [[ -f "configure.ac" ]] ; then
  function setup_dirs ()
  {
    local rootdir
    rootdir="${MAS_MAS_DIR}/develop/autotools/zoc/"
    export LD_LIBRARY_PATH=/usr/local/lib

    if [[ -d "$rootdir" ]] ; then
      savedir=$( realpath "$rootdir/saved/" )

      tmpdir="$rootdir/tmp/"
      wd=`pwd`
      me="$wd/$0"
      shdir=$( dirname "$me" )
      shdirup=$( dirname $shdir )
      indir="$( realpath "$shdirup" )"
      updir="$( realpath $indir/.. )"
      debugdir=$indir/debug
     #savedir=$updir
      # name=$( basename $indir )
      if [[ -f "$indir/configure" ]] ; then
	name=$( cd "$indir" && ./configure -V|head -1|awk '{print $1}' )
	ver=$( cd "$indir" && ./configure -V|head -1|awk '{print $3}' )
      fi
      workdir=$tmpdir
      if [[ "$name" ]] && [[ "$ver" ]] ; then
	unpackdir="$workdir/${name}-${ver}"
	builddir=$workdir/build/$name
      fi
    fi
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
       env | grep MAS_ZOCROMAS >&2

    # for core dump:
       ulimit -c unlimited

  #     make_any && usleep 500000 && clear && exec $srcdir/$rname "$@"
       make_any && usleep 50000 && exec $srcdir/$rname "$@"
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
    local ername
    ername=errors/make.`datemt`.result
    if [[ "$ername" ]] ; then
      if ! [[ -f "$ername" ]] ; then
	if make -s $@ >$ername 2>&1 ; then
	  rm $ername
	  return 0
	else
	  if [[ -s "$ername" ]] ; then
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
  setup_dirs
else
  echo "no project in this directory" >&2
  exit
fi
