#!/bin/sh
if [[ -f "sh/make_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/make_functions.sh
  
# echo "PKG_CONFIG_PATH: $PKG_CONFIG_PATH" >&2
  make_target $@
fi

