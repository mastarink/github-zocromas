#!/bin/sh
dir=$( realpath $( dirname $0 ) )
indir="$( realpath $dir/.. )"
updir="$( realpath $indir/.. )"
savedir=$updir/..
# ver=1.0.1.2
ver=$( cd $indir && ./configure -V|head -1|awk '{print $3}' )
workdir=$( realpath $dir/../../../tmp )
# name=$( basename $indir )
name=$( cd $indir && ./configure -V|head -1|awk '{print $1}' )
distfile="$indir/${name}-${ver}.tar.bz2"
unpackdir="$workdir/${name}-${ver}"
builddir=$workdir/build/$name

if cd "$indir" ; then
# make -d dist
# make -s dist
  make -s distcheck
  mv *.tar.{bz2,gz} $savedir

  if cd "$workdir" ; then
    if [[ -d "$builddir" ]] ; then
      rm -Rf "$builddir" || exit
    fi
    mkdir -p "$builddir" || exit

    if [[ -d "$unpackdir" ]] ; then
      rm -Rf "$unpackdir"
    fi
    cd .
    if [[ -f "$distfile" ]] ; then
      tar -jxf "$savedir/$distfile"
      if cd $builddir ; then
#	pwd
      # ls -l ../zocromas-*/configure
      # sleep 5
	if [[ -f "$unpackdir/configure" ]] ; then
	  $unpackdir/configure --silent
	  make -s
      # ls -lAtr src
        fi
      fi
    else
      echo "No file: $distfile" >&2
    fi
  else
    echo "No dir: $workdir" >&2
  fi
else
  echo "No dir: '$indir'" >&2
fi
echo "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-">&2
echo "$unpackdir">&2
echo "TODO as root:">&2
echo "cd `pwd` && make install" >&2
echo "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-">&2
