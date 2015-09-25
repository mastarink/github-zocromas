#!/bin/sh
dir="$( realpath $( dirname $0 ) )"
indir="$( realpath $dir/.. )"
updir="$( realpath $indir/.. )"
name=$( basename $indir )
if [[ "$updir" ]] && [[ -d "$updir" ]] ; then
  cd $updir
  if [[ -d "$indir/log" ]] ; then
    rm -f $indir/log/${name}.*.log
  fi
 (
   cd $indir
   make -s distclean 
 )
  mv $indir/*.{bz2,gz} $updir
  if [[ "$name" ]] && [[ -d "$name" ]] ; then
    tar --wildcards --exclude '.*.sw*' --exclude '*.sqlite3' --exclude-caches --exclude-tag mas-tar-exclude -jvcf $name.`datemt`.tar.bz2 $name/
  fi
  cd $indir
  autoreconf
  ./configure --silent
  sh/make.sh
fi
