#!/bin/sh
if [[ -f "sh/build_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/build_functions.sh
 
  testdist && makd_dinst_script
fi
