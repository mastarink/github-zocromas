
# Configure options: --enable-debug[=no].

AC_ARG_ENABLE([debug],
  [AS_HELP_STRING([--enable-debug],
    [enable debug code (default is no)])],
  [debug="$withval"], [debug=no])

AC_ARG_ENABLE([debug],
	      AC_HELP_STRING([--enable-debug],
			     [turn on debugging]),,
	      [enable_debug=no])



# Configure options: --enable-mas-test[=no].

AC_ARG_ENABLE([mas_test],
	      AC_HELP_STRING([--enable-mas-test],
			     [turn on testing]),,
	      [enable_mas_test=no])
AM_CONDITIONAL([MAS_TEST], [test x$enable_mas_test = xyes])



AC_ARG_ENABLE([gprof],
	      AC_HELP_STRING([--enable-grof],
			     [turn on gprof flags]),
  [enable_gprof="$withval"], [enable_gprof=no])

if test x$enable_gprof = xyes ; then
  AM_CFLAGS="${AM_CFLAGS} -pg -O0 -g -ggdb"
else
  if test x$enable_debug = xyes ; then
    AM_CFLAGS="${AM_CFLAGS} -O0 -g -ggdb"
  fi
fi

if test x$enable_debug = xyes ; then
    AC_DEFINE(MAS_DEBUG, 1,
	[Enable additional debugging at the expense of performance and size])
fi


# AH_TEMPLATE([MAS_MODULESDIR],[mas_modulesdir])
# MAS_MODULESDIR="\${pkglibdir}"
# AC_SUBST(MAS_MODULESDIR)

# AH_TEMPLATE([MAS_LIBDIR],[mas_libdir])
# MAS_LIBDIR="\${libdir}"
# AC_SUBST(MAS_LIBDIR)


