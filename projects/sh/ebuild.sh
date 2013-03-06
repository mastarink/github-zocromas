#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  . sh/build_functions.sh
  ebuild_m $@
fi
