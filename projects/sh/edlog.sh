#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  ! type -t gvim_caller && . $MAS_SHLIB/libstdbins.bash

  if [[ "$MAS_ZOCROMAS_HERE" ]] ; then
    if [[ "$MAS_PERSONAL_EDITOR" ]] ; then
      $MAS_PERSONAL_EDITOR $( ls -1tr $logdir/${MAS_ZOCROMAS_HERE}.*|tail -1 )
    fi
  else
    echo "Pleasem set " >&2
    echo "  export MAS_ZOCROMAS_HERE=server" >&2
    echo "    or" >&2
    echo "  export MAS_ZOCROMAS_HERE=client" >&2
  fi
fi
