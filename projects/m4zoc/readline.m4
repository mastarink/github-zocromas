AC_ARG_ENABLE([readline],
	      [AS_HELP_STRING([--disable-readline],[do not compile code that uses the libreadline library])],,
	      [enable_readline=yes])
if test "x$enable_readline" = "xyes"; then
	SAVE_CFLAGS="$CFLAGS"
	CFLAGS=""
	AC_CHECK_LIB(readline, readline,
        	     [enable_readline=yes],
	     	     [enable_readline=no])
	if test "x$enable_readline" = "xyes"; then
		AC_DEFINE(HAVE_LIBREADLINE, 1, [Define to 1 if libreadline support is included])
		READLINE_LIBS='-lreadline'
	fi
	CFLAGS="$SAVE_CFLAGS"
	unset SAVE_CFLAGS
fi
AC_SUBST(READLINE_LIBS)
AM_CONDITIONAL(ENABLE_READLINE, test "x$enable_readline" = xyes)



