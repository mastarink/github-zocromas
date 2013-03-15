#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  . sh/run_functions.sh
  
  sig=$1
  shift
  if ! [[ "$sig" ]] ; then sig=QUIT ; fi
  spid=$( server_pid )
  if [[ "$spid" ]] ; then
    kill -$sig $spid
  fi
fi
