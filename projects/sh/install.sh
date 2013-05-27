#!/bin/sh
if [[ -f "sh/libmake_functions.bash" ]] ; then
  . sh/setup.sh
  . sh/libmake_functions.bash

  make_m && make_target install $@
fi
