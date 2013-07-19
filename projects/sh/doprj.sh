#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/libdo_functions.bash" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  . ${MAS_PROJECTS_DIR:-.}/sh/libdo_functions.bash

  doprj $@
fi
