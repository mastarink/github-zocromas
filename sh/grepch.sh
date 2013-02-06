#!/bin/sh
. $MAS_MAS_DIR/develop/autotools/zoc/sh/setup.sh
if [[ "$rootdir" ]] && [[ -d "$rootdir" ]] ; then
  cd $rootdir
  grep --color=yes -r --inc=*.[ch] "$@" {commonlibs,zocromas,zocmds}
fi
