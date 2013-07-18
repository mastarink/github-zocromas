#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/libmake_functions.bash" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  . ${MAS_PROJECTS_DIR:-.}/sh/libmake_functions.bash
  
# echo "PKG_CONFIG_PATH: $PKG_CONFIG_PATH" >&2
  make_any_tilme $@
fi
