#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/libmake_functions.bash" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  . ${MAS_PROJECTS_DIR:-.}/sh/libmake_functions.bash

  make_m && make_target install $@
fi
