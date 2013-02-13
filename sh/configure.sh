#!/bin/sh

if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  echo "rootdir      :$rootdir" >&2
  echo "indir        :$indir" >&2
  echo "configuredir :$configuredir" >&2
  echo "ibuilddir    :$ibuilddir" >&2
  echo "wbuilddir    :$wbuilddir" >&2
  echo "At `pwd`" >&2

  configure $@
fi
