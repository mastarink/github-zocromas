

AC_ARG_ENABLE([tracemem],
	      AC_HELP_STRING([--enable-tracemem],
			     [turn on tracememing]),,
	      [enable_tracemem=no])

if test x$enable_tracemem = xyes ; then
    AC_DEFINE(MAS_TRACEMEM, 1,
	[Enable tracememing at the expense of performance and size])
fi
########### ctrl   ########################################################################
AC_ARG_ENABLE([noctrl],
	      AC_HELP_STRING([--enable-noctrl],
			     [turn off ctrl]),,
	      [enable_noctrl=no])

if test x$enable_noctrl = xyes ; then
    AC_DEFINE(MAS_NO_CTRL, 1,
	[Enable ctrl-less prog.])
fi
########### opts   ########################################################################
AC_ARG_ENABLE([noopts],
	      AC_HELP_STRING([--enable-noopts],
			     [turn off opts]),,
	      [enable_noopts=no])

if test x$enable_noopts = xyes ; then
    AC_DEFINE(MAS_NO_OPTS, 1,
	[Enable opts-less prog.])
fi
########### thtools   ########################################################################
AC_ARG_ENABLE([nothtools],
	      AC_HELP_STRING([--enable-nothtools],
			     [turn off thtools]),,
	      [enable_nothtools=no])

if test x$enable_nothtools = xyes ; then
    AC_DEFINE(MAS_NO_THTOOLS, 1,
	[Enable thtools-less prog.])
fi
#############################################################################################

# AC_REQUIRE([AM_MAINTAINER_MODE])
#
# if test $USE_MAINTAINER_MODE = yes; then
# ..........
# fi

