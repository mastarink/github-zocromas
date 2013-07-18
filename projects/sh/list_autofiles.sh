#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/setup.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  if cd $indir && [[ "$make_errdir" ]] && [[ -d "$make_errdir" ]] ; then
    ls -1 INSTALL COPYING ltmain.sh missing install-sh depcomp config.sub config.guess config.h.in~ vrb.tmp ac.mased.viminfo src.mased.viminfo aclocal.m4 configure Makefile.in config.status stamp-h1 $make_logname Makefile libtool config.log config.h compile m4/* autom4te.cache/* src/.libs/* src/.deps/* src/*.{la,lo,o} src/Makefile.in src/Makefile src/inc/Makefile.in src/inc/Makefile *.pc $make_errdir/*  2>/dev/null | grep -v '\/mas-tar-exclude$'
  fi
fi
