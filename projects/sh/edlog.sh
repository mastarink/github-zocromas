#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/setup.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  ! type -t gvim_caller && . $MAS_SHLIB/libstdbins.bash
    
# run_any ${MAS_ZOCROMAS_HERE:-.} "$@"
  echo "[$rname_preset]" >&2
  echo "[$binprefix]" >&2
  echo "[$mas_name]" >&2
  echo "[$prjname]" >&2
  echo "[$short_name]" >&2
  lastlog=$( ls -1tr $logdir/${short_name}*|tail -1 )
  echo "[$lastlog]" >&2
  if [[ "$lastlog" ]] && [[ -f "$lastlog" ]] ; then
    $MAS_PERSONAL_EDITOR $lastlog
  elif [[ "$MAS_ZOCROMAS_HERE" ]] ; then
    if [[ "$MAS_PERSONAL_EDITOR" ]] ; then
      $MAS_PERSONAL_EDITOR $( ls -1tr $logdir/${MAS_ZOCROMAS_HERE}.*|tail -1 )
    fi
  else
    echo "Please set " >&2
    echo "  export MAS_ZOCROMAS_HERE=server" >&2
    echo "    or" >&2
    echo "  export MAS_ZOCROMAS_HERE=client" >&2
  fi
fi
