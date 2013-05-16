#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  . sh/librun_functions.bash
  
  eddiffconfig
fi
