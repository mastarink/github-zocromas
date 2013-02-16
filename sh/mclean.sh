#!/bin/sh
if [[ -f "sh/make_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/make_functions.sh

  make_target clean $@
fi

