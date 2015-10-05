AM_PROG_AR
AM_INIT_AUTOMAKE([foreign  silent-rules -Wall -Wextra -Werror dist-bzip2])
dnl AM_INIT_AUTOMAKE([foreign  -Wall -Werror dist-bzip2 nostdinc])

dnl AM_MAINTAINER_MODE([enable])

dnl AM_EXTRA_RECURSIVE_TARGETS([foo])
AM_SILENT_RULES([yes])

dnl Checks for programs.
dnl AC_PROG_GCC_DEFAULT_FLAGS([-O0 -Wall])

dnl To turn off '-g -O2' :
dnl ${CFLAGS="-O2"}
dnl # CFLAGS="-Os"
dnl ${CFLAGS="-O0 -g -ggdb"}
dnl CFLAGS="-O0 -g -ggdb"
CFLAGS=""
AC_SUBST([CFLAGS])
CXXFLAGS=""
AC_SUBST([CXXFLAGS])

AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O

dnl AM_PROG_CC_STDC
dnl AC_C_CONST


