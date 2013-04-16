#!/bin/sh
if type -t zoc_error >/dev/null 2>&1 ; then
  :
elif [[ -f "configure.ac" ]] || [[ -f "config.status" ]] ; then
  . sh/setupz.sh
elif ! [[ "$MAS_OK_NO_PROJECT" ]] ; then
  echo "no project in this directory - call it from any project dir" >&2
  if ! [[ "$0" == `which bash` ]] ; then
    echo "@@@@ $0 @@@@" >&2
#   exit
  fi
fi
