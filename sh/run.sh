#!/bin/sh
if [[ -f "sh/run_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/run_functions.sh

# if [[ "$MAS_ZOCROMAS_HERE" ]] ; then
    run_any ${MAS_ZOCROMAS_HERE:-.} "$@"
# else
#   echo "Please set " >&2
#   echo "  export MAS_ZOCROMAS_HERE=server" >&2
#   echo "    or" >&2
#   echo "  export MAS_ZOCROMAS_HERE=client" >&2
#   # ls -l $instdir
# fi
fi
