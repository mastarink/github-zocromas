#!/bin/sh
. ~/.mas/develop/autotools/sh/setup.sh

timname=make-timing.`datemt`.result
if [[ "$timname" ]] ; then
  if ! [[ -f "$timname" ]] ; then
    tstart=`date`
    make_any $@ > $timname 2>&1
    echo "$tstart; $( grep '^real' $timname | sed -ne 's/^real\s*//p' )" >> makes.log
    cat "$timname"
    rm "$timname"
  fi
fi
