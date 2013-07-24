#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/setup.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  . ${MAS_PROJECTS_DIR:-.}/sh/librun_functions.bash
  
  sig=$1
  shift
  if ! [[ "$sig" ]] ; then sig=QUIT ; fi
  if spid=$( server_pid ) && [[ "$spid" ]] && [[ "$sig" ]] ; then
    echo "[[ killing $spid with $sig ]]" >&2
    kill -$sig $spid && echo "KILLED $spid with $sig" >&2 || echo "NOT killed $spid with $sig"
  else
    echo "No server pid" >&2
  fi
  sleep 0.5
  psshow
fi
