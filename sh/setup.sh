#!/bin/sh
if [[ -f "configure.ac" ]] ; then
  . sh/setupz.sh
elif [[ -f "config.status" ]] ; then
  . sh/setupz.sh
elif ! [[ "$MAS_OK_NO_PROJECT" ]] ; then
  echo "no project in this directory - call it from any project dir" >&2
  exit
fi
