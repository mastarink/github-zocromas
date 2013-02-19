#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  if [[ "$rootdir" ]] && [[ -d "$rootdir" ]] ; then
    echo find at "$rootdir" >&2
#   find "$rootdir" -name '*.mased.vim' | sed -ne 's/^\(.*\)\/mased\/[^\/]\+$/\1/p' | sed -ne "s@${rootdir}\/@@p" | sort | uniq
    find "$rootdir" -name 'configure.ac' | sed -ne 's/^\(.*\)\/[^\/]\+$/\1/p' | sed -ne "s@${rootdir}\/@@p" \
    		| grep -v '^\(apue\|testx\|install\|templates\|tmp\|commonlibs\/mas_maslibtemplate\)\>' | sort | uniq
  else
    echo "rootdir not set ($rootdir)" >&2
  fi
fi
