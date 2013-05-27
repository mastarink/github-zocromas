#!/bin/sh
if [[ -f "sh/libmake_functions.bash" ]] ; then
  . sh/setup.sh
  . sh/libmake_functions.bash
  
# echo "PKG_CONFIG_PATH: $PKG_CONFIG_PATH" >&2
  make_target $@
fi

