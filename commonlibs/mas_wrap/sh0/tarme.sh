#!/bin/sh
dir="$( realpath $( dirname $0 ) )"
indir="$( realpath $dir/.. )"
updir="$( realpath $indir/.. )"
savedir=$updir/..
name=$( basename $indir )
ver=$( cd $indir && ./configure -V|head -1|awk '{print $3}' )
if [[ "$updir" ]] && [[ -d "$updir" ]] ; then
  cd $updir
  echo "rm $indir/log/server.*.log" >&2
  echo "rm $indir/log/client.*.log" >&2
  if [[ -d "$indir/log" ]] ; then
    rm -f $indir/log/server.*.log
    rm -f $indir/log/client.*.log
  fi
 (
   cd $indir
   make -s distclean 
 )
  mv $indir/*.{bz2,gz} $savedir
  if [[ "$name" ]] && [[ -d "$name" ]] ; then
    tar --wildcards --exclude '.*.sw*' --exclude-caches --exclude-tag mas-tar-exclude -jvcf $name.$ver.`datemt`.tar.bz2 $name/
  fi
  mv *.tar.{bz2,gz} $savedir
 (
  cd $indir
  autoreconf
  ./configure --silent
  sh/make.sh
 )
fi
