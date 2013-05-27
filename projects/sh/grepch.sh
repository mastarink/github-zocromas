#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  . sh/libdir_functions.bash

  grepch $@
fi
