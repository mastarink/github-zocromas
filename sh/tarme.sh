#!/bin/sh
. $MAS_MAS_DIR/develop/autotools/zoc/sh/setup.sh
# show_setup

if [[ "$name" ]] && [[ "$ver" ]] ; then
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
#   echo "Saving $indir/*.{bz2,gz} to $savedir" >&2
#   mv $indir/*.{bz2,gz} "$savedir"
    if [[ "$name" ]] && [[ -d "$name" ]] ; then
      tar --wildcards --exclude '.*.sw*' --exclude-caches --exclude-tag mas-tar-exclude -jvcf $name.$ver.`datemt`.tar.bz2 $name/
    else
      echo "name: $name; pwd : `pwd`" >&2
    fi
    echo "Saving *.tar.{bz2,gz} to $savedir" >&2
    mv *.tar.bz2 "$savedir"    
   (
    echo "Change to $indir" >&2
    cd "$indir"
    autoreconf
    ./configure --enable-tracemem --enable-debug --silent
    sh/make.sh
   )
  fi
fi
