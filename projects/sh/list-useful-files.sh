#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/setup.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  ls -1 {{zocromas,zocmds,zoctypes,commonlibs/mas_{channel,fileinfo,io,log,modules,msg,thtools,tools,variables,wrap}}/{gvim-*,{g,}vimrc-*,missing,depcomp,configure,config.{h,guess,status,sub},libtool,install-sh,libtool,ltmain.sh,Makefile{.in,.am,},*.{ac,m4},m4/*,mased/*,{src,inc}/{Makefile{.in,.am,},*.[ch],inc/{Makefile{.in,.am,},*.h}}},zocromas/src/*/{Makefile{.in,.am,},*.c,inc/{Makefile{.in,.am,},*.h}}} sh/*.sh 2>/dev/null | sort
fi
