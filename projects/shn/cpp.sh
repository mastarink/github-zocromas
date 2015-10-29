#!/bin/sh
function docpp ()
{
  local file=$1
  local bname=$(basename $file)
  local dir=$(dirname $file)
  if [[ $bname == *.c ]] ; then
    local cname=${bname%.c}
    local ofile=${cname}-i.c
    if [[ -f $file ]] && [[ -s $file ]] && [[ ${MSH_SHN_DIRS[flavour]} ]] ; then
      echo gcc -include .auxdir/.build/config.h -Isrc/inc  -isystem ${MSH_SHN_DIRS[flavour]}/include -isystem /usr/include  -E -P  $file -o $ofile >&2
      gcc -include .auxdir/.build/config.h -Isrc/inc  -isystem ${MSH_SHN_DIRS[flavour]}/include -isystem /usr/include  -E -P  $file -o $ofile
    else
      echo "$file :: ${MSH_SHN_DIRS[flavour]}" >&2
    fi
  fi
}
if [[ -L shn ]] && [[ -f shn/libwork.bash ]] ; then
  . shn/libwork.bash
  docpp $@
fi
