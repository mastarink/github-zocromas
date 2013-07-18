#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/build_functions.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  . ${MAS_PROJECTS_DIR:-.}/sh/build_functions.sh
 
  testdist_m && makd_dinst_script
fi
