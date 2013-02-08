#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  n=$(prjconfname)
  v=$(prjconfversion)
  echo "  $( basename $( realpath . ) ); { $n = $v }"
  pwd
fi
