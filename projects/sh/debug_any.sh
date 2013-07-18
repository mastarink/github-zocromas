#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/debug_functions.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  . ${MAS_PROJECTS_DIR:-.}/sh/debug_functions.sh

  debug_any $@
fi
