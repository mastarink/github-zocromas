#!/bin/bash
. ${MAS_SHN_DIR:-shn}/draft1.sh
. ${MAS_SHN_DIR:-shn}/libsetup.bash



for (( i=0 ; $i < ${#BASH_SOURCE[@]} ; i++ )) ; do
  echo "${i}. a src1: ${BASH_SOURCE[$i]} : ${FUNCNAME[$i]}" >&2
done

if false ; then
  draft_a A $@
  echo "====================" >&2
  draft_a B $@
  echo "====================" >&2
fi

