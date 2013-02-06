#!/bin/sh
. $MAS_MAS_DIR/develop/autotools/zoc/sh/setup.sh

function configure ()
{
  if [[ "$indir" ]] && [[ -d "$indir" ]] ; then
    cd $indir
    if [[ -d "$instdir" ]] ; then
      ./configure --prefix="$rootdir/install" --silent --enable-silent-rules --enable-tracemem --enable-debug || return 1
    else
  #   ./configure --silent --enable-silent-rules --enable-tracemem --enable-debug
      echo "no dir $instdir" >&2
      return 1
    fi
  fi
  return 0
}
configure $@
