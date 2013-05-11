#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  . sh/run_functions.sh
  
  sig=$1
  shift
  if ! [[ "$sig" ]] ; then sig=QUIT ; fi
  if spid=$( server_pid ) && [[ "$spid" ]] ; then
    echo "killing $spid with $sig" >&2
    kill -$sig $spid
  else
    echo "No server pid" >&2
  fi
  psshow
fi
