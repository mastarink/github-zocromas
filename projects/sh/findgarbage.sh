#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

# make clean distclean 2>/dev/null
  find src/ -name '*.o' -o -name '*.la' -o -name '*.lo'  -o -name '*.Po' -o -name '*.Plo'
fi