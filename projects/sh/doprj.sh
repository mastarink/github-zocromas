#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/do_functions.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  . ${MAS_PROJECTS_DIR:-.}/sh/do_functions.sh

  doprj $@
fi
