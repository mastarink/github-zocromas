#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  if [[ "$rootdir" ]] && [[ -d "$rootdir" ]] ; then
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
	-r --color=yes $@ $rootdir
  fi
fi
