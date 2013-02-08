#!/bin/sh
. $MAS_MAS_DIR/develop/autotools/zoc/sh/setup.sh
if [[ "$indir" ]] && [[ -d "$indir" ]] ; then
  cd $indir
  if ! [[ -d m4 ]] ; then
    mkdir m4
  fi
  make -s maintainer-clean
  autoreconf -i
fi
