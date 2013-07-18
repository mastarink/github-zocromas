#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/setup.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh

  if [[ -f "$indir/zocversion.txt" ]] ; then
    vseq=$( cat $indir/zocversion.txt )
    if type -t datem 2>&1 >/dev/null ; then
      echo -n "${vseq}.$( datem )"
    else
      echo -n "$vseq"
    fi
  else
    echo -n '0.0.1'
  fi
fi
