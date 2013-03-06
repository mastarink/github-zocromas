#!/bin/sh
if [[ -f "sh/debug_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/debug_functions.sh

  debug_any $@
fi
