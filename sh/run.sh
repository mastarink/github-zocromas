#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  if [[ "$MAS_ZOCROMAS_HERE" ]] ; then
    run_any ${MAS_ZOCROMAS_HERE} "$@"
  else
    echo "Pleasem set " >&2
    echo "  export MAS_ZOCROMAS_HERE=server" >&2
    echo "    or" >&2
    echo "  export MAS_ZOCROMAS_HERE=client" >&2
  fi
fi
