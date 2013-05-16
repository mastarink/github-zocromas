#!/bin/sh
if [[ -f "sh/librun_functions.bash" ]] ; then
  . sh/setup.sh
  . sh/librun_functions.bash

  run_any $0 "$@"
fi
