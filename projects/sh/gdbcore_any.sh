#!/bin/sh
. sh/debug_functions.sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  gdb_core_any $@ 
fi


# libtool --mode=execute gdb  -ex 'set print thread-events off' -x debug/debug_zocromas_server.pid.cmd .auxdir/.build/src/zocromas_server 19687
# libtool --mode=execute gdb  -ex 'set print thread-events off' -x debug/debug_zocromas_server.pid.cmd .auxdir/.build/src/zocromas_server `ps -CzocDaeMaster -o pid=`
# strace -q -fr -C -p 18033

