#!/bin/sh
if [[ -f "sh/librun_functions.bash" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  . ${MAS_PROJECTS_DIR:-.}/sh/librun_functions.bash

# if [[ "$MAS_ZOCROMAS_HERE" ]] ; then
#    echo "RUN START" >&2
    run_any ${MAS_ZOCROMAS_HERE:-.} "$@"
#    echo "RUN END" >&2
# else
#   echo "Please set " >&2
#   echo "  export MAS_ZOCROMAS_HERE=server" >&2
#   echo "    or" >&2
#   echo "  export MAS_ZOCROMAS_HERE=client" >&2
#   # ls -l $flavourdir
# fi
fi
