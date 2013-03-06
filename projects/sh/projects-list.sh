#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  if [[ "$projectsdir" ]] && [[ -d "$projectsdir" ]] ; then
    echo find at "$projectsdir" >&2
#   find "$projectsdir" -name '*.mased.vim' | sed -ne 's/^\(.*\)\/mased\/[^\/]\+$/\1/p' | sed -ne "s@${projectsdir}\/@@p" | sort | uniq
    find "$projectsdir" -name 'configure.ac' | sed -ne 's/^\(.*\)\/[^\/]\+$/\1/p' | sed -ne "s@${projectsdir}\/@@p" \
    		| grep -v '^\(apue\|testx\|install\|templates\|tmp\|commonlibs\/mas_maslibtemplate\)\>' | sort | uniq
  else
    echo "projectsdir not set ($projectsdir)" >&2
  fi
fi
