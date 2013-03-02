#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setupz.sh
  . sh/run_functions.sh

  psshow
fi
