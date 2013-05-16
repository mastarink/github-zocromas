#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  . sh/librun_functions.bash
find . src src/inc inc \
	 -maxdepth 1 \
     \( \
	-name aclocal.m4 -o \
	-name autom4te.cache -o \
	-name compile -o \
	-name config.guess -o \
	-name config.h.in~ -o \
	-name config.h -o \
	-name config.log -o \
	-name config.status -o \
	-name config.sub -o \
	-name configure -o \
	-name depcomp -o \
	-name install-sh -o \
	-name libtool -o \
	-name ltmain.sh -o \
	-name Makefile.in -o \
	-name Makefile -o \
	-name Makefile -o \
	-name "$make_logname" -o \
	-name missing -o \
	-name stamp-h1 -o \
	-name '*.viminfo' -o \
	-name '*.tmp' -o \
	-name '*.{la,lo,o}' -o \
	-name '*.pc' -o \
	\
	\( -type d -name m4 \) -o \
	\( -type d -name '.build' \) -o \
	\( -type d -name '.autom4te.cache' \) \
     \) 
#
#	2>/dev/null | grep -v '\/mas-tar-exclude$'

fi
