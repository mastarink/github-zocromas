#!/bin/sh
if [[ -f "sh/libmake_functions.bash" ]] ; then
  . sh/setup.sh
  . sh/libmake_functions.bash

  make_target uninstall $@
fi
