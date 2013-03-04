#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  port=5005
  if [[ "$1" ]] ; then
    port=$1
    shift
  fi
  if [[ -d "$testdir" ]] ; then
    /usr/sbin/ab2 -c5 -n10000  http://zocromas.mastar.lan:$port/HarryHarrison.jpeg > "$testdir/${now_stamp}.ab2" 2>&1
#grep '^\(Time\sper\srequest:.*(mean)\)'  $fname
  fi
  if [[ -f $testdir/${now_stamp}.ab2 ]] ; then
    cat $testdir/${now_stamp}.ab2
  fi
fi
