#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  if [[ "$rootdir" ]] && [[ -d "$rootdir" ]] ; then
    if [[ "$1" ]] ; then 
      find "$rootdir" -type f -name "$@"
    else
      find "$rootdir" -type f -name '*.[ch]'
    fi \
	| sed -ne "s@${rootdir}\/@@p" \
	| grep -v '^\(apue\|testx\|install\|templates\|tmp\|commonlibs\/mas_maslibtemplate\)\>' \
	| grep -v '\(\/\.build\/\|zocromas\/misc\/\)'
  fi
fi
