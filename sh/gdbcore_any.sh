#!/bin/sh
. sh/debug_functions.sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  gdb_core_any $@ 
fi
