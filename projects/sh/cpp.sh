#!/bin/sh
. ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
. ${MAS_PROJECTS_DIR:-.}/sh/debug_functions.sh
function docpp ()
{
  local incl
  if [[ "$flavourdir" ]] && [[ -d "$flavourdir" ]] ; then
    incl="-I$flavourdir/include"
    if [[ "$MAS_PROJECT_DIR" ]] && [[ -d "$MAS_PROJECT_DIR/src/inc" ]] ; then
      incl="$incl -I$MAS_PROJECT_DIR/src/inc"
    fi
    cpp $incl $@
  fi
}
docpp $@
