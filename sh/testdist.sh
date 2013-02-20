#!/bin/sh
if [[ -f "sh/build_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/build_functions.sh
 
  testdist_m && makd_dinst_script
fi
