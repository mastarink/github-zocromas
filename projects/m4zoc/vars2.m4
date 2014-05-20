
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


AC_ARG_ENABLE([split_db],
  [AS_HELP_STRING([--enable-split-db],
    [enable split-db code (default is no)])],
  [split_db="$withval"], [split_db=no])

AC_ARG_ENABLE([split_db],
	      AC_HELP_STRING([--enable-split-db],
			     [turn on split-db]),,
	      [enable_split_db=no])
if test x$enable_split_db = xyes ; then
    AC_DEFINE(MAS_SPLIT_DB, 1,
	[Enable  split-db])
fi







# AH_TEMPLATE([MAS_MODULESDIR],[mas_modulesdir])
# MAS_MODULESDIR="\${pkglibdir}"
# AC_SUBST(MAS_MODULESDIR)

# AH_TEMPLATE([MAS_LIBDIR],[mas_libdir])
# MAS_LIBDIR="\${libdir}"
# AC_SUBST(MAS_LIBDIR)


