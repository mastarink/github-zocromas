#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/setup.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh

# make clean distclean 2>/dev/null
  find src/ -name '*.o' -o -name '*.la' -o -name '*.lo'  -o -name '*.Po' -o -name '*.Plo'
fi
