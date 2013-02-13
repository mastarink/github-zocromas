#!/bin/sh

if [[ -f "sh/setup.sh" ]] ; then
  . sh/setupz.sh
  wbuilddir=$ibuilddir
  if [[ "$unpackdir" ]] && [[ -f "$unpackdir/configure" ]] ; then
    configuredir=$unpackdir
  elif [[ "$indir" ]] && [[ -f "$indir/configure" ]] ; then
    configuredir=$indir
  fi

  echo "rootdir      :$rootdir" >&2
  echo "indir        :$indir" >&2
  echo "configuredir :$configuredir" >&2
  echo "unpackdir    :$unpackdir" >&2
  echo "ibuilddir    :$ibuilddir" >&2
  echo "wbuilddir    :$wbuilddir" >&2
  echo "workdir      :$workdir" >&2
  echo "At `pwd`" >&2
  make_any
fi
