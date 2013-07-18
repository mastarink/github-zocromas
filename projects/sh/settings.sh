#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/librun_functions.bash" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh

  show_setup
fi
