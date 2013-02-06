#!/bin/sh
. $MAS_MAS_DIR/develop/autotools/zoc/sh/setup.sh

function install ()
{
  local ilog
# echo "install [$prjname]" >&2
  ilog="/tmp/install-$prjname.log"
  if make -s install > $ilog ; then
    echo "$prjname install OK" >&2
  else
    echo "$prjname install FAIL" >&2
    cat "$ilog" >&2
  fi
}
install $@
