#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  make_target distclean $@
fi


