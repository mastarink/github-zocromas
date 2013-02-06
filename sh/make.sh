#!/bin/sh
. $MAS_MAS_DIR/develop/autotools/zoc/sh/setup.sh

function make_any_tilme ()
{
  local timname tstart
  timname=make-timing.`datemt`.result
  if [[ "$timname" ]] ; then
    if ! [[ -f "$timname" ]] ; then
      tstart=`date`
      if make_any $@ > $timname 2>&1 ; then
	echo "$tstart; $( grep '^real' $timname | sed -ne 's/^real\s*//p' )" >> makes.log
	cat "$timname"
	rm "$timname"
      else
	echo "$tstart; $( grep '^real' $timname | sed -ne 's/^real\s*//p' )" >> makes.log
	cat "$timname"
	rm "$timname"
	return 1
      fi
    fi
  fi
  return 0
}
make_any_tilme $@
