#!/bin/sh
if [[ -f "sh/make_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/make_functions.sh

  make_m && make_target install $@
fi
