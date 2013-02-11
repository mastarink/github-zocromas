#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  if [[ -f "$rootdir/zocversion.txt" ]] ; then
    if type -t datemt 2>&1 >/dev/null ; then
      echo -n "$( cat $rootdir/zocversion.txt ).$( datemt )"
    else
      echo -n "$( cat $rootdir/zocversion.txt )"
    fi
  else
    echo -n '0.0.1'
  fi
fi
