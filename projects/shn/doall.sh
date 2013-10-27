#!/bin/sh
if [[ -L shn ]] && [[ -f shn/libwork.bash ]] ; then
  . shn/libwork.bash
  shn each $@
fi
