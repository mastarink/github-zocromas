#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  if [[ "$projectsdir" ]] && [[ -d "$projectsdir" ]] ; then
    grep --binary-files=without-match \
    	--exclude='*.viminfo' \
	--exclude='output.*' \
	--exclude='*.log' \
	--exclude='*.tmp' \
	--exclude='Makefile' \
	--exclude='*.bz2' \
	--exclude='*.gz' \
	--exclude='*.o' \
	--exclude='*.so' \
	--exclude='*.so.*' \
	--exclude='*.a' \
	--exclude='*.la' \
	--exclude='*.lai' \
	--exclude='*.Plo' \
	--exclude='*.Po' \
	--exclude='config.status' \
	--exclude='*.result' \
	-r --color=yes $@ $projectsdir
  fi
fi
