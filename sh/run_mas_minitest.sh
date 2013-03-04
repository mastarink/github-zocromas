#!/bin/sh
if [[ -f "sh/run_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/run_functions.sh

  run_any $0 "$@"
fi
