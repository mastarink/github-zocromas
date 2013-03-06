#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  if [[ "$projectsdir" ]] && [[ -d "$projectsdir" ]] ; then
    if [[ "$1" ]] ; then 
      find "$projectsdir" -type f -name "$@"
    else
      find "$projectsdir" -type f -name '*.[ch]'
    fi \
	| sed -ne "s@${projectsdir}\/@@p" \
	| grep -v '^\(apue\|testx\|install\|templates\|tmp\|commonlibs\/mas_maslibtemplate\)\>' \
	| grep -v '\(\/\.build\/\|zocromas\/misc\/\)'
  fi
fi
