#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  . sh/run_functions.sh
  
  eddiffconfig
fi
