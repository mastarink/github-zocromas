#!/bin/sh

if [[ -L shn ]] && [[ -f shn/libwork.bash ]] ; then
  . shn/libwork.bash
  sname=$1
  if [[ $sname =~ (src|src/inc)/(.[a-z_]+)\.c ]] ; then
    pref=${BASH_REMATCH[1]}
    name=${BASH_REMATCH[2]}
    echo -- make -C  ${MSH_SHN_DIRS[build]}/$pref -f ${MSH_SHN_DIRS[build]}/$pref/Makefile ${MSH_SHN_PROJECT_NAME}-${name}.o
    make -C          ${MSH_SHN_DIRS[build]}/$pref -f ${MSH_SHN_DIRS[build]}/$pref/Makefile ${MSH_SHN_PROJECT_NAME}-${name}.o
    ls -l            ${MSH_SHN_DIRS[build]}/$pref/${MSH_SHN_PROJECT_NAME}-${name}.o
  else
    echo "[$sname] ?"
  fi
fi
