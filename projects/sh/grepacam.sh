#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/setup.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  if [[ "$projectsdir" ]] && [[ -d "$projectsdir" ]] ; then
    cd $projectsdir
    grep --color=yes -r --inc='*.ac' --inc='*.am' "$@" {commonlibs,bins,zoc*}
  fi
fi

