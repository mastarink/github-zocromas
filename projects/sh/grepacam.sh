#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  if [[ "$projectsdir" ]] && [[ -d "$projectsdir" ]] ; then
    cd $projectsdir
    grep --color=yes -r --inc='*.ac' --inc='*.am' "$@" {commonlibs,bins,zoc*}
  fi
fi

