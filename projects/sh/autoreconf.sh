#!/bin/sh
if [[ -f "sh/make_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/build_functions.sh
  
  autoreconf_m
fi
