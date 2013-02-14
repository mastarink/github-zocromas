#!/bin/sh
if [[ -f "sh/do_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/do_functions.sh

  doall $@
fi
