#!/bin/sh
if [[ -f "sh/libmake_functions.bash" ]] ; then
  . sh/setup.sh
  . sh/build_functions.sh

  configure_m `realpath .` $@
fi
