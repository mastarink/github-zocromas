AC_ARG_ENABLE([magic],
	      [AS_HELP_STRING([--disable-magic],[do not compile code that uses the libmagic library])],,
	      [enable_magic=yes])
if test "x$enable_magic" = "xyes"; then
	SAVE_CFLAGS="$CFLAGS"
	CFLAGS=""
	AC_CHECK_LIB(magic, magic_open,
        	     [enable_magic=yes],
	     	     [enable_magic=no])
	if test "x$enable_magic" = "xyes"; then
		AC_DEFINE(HAVE_LIBMAGIC, 1, [Define to 1 if libmagic support is included])
		MAGIC_LIBS='-lmagic -lm'
	fi
	CFLAGS="$SAVE_CFLAGS"
	unset SAVE_CFLAGS
fi
AC_SUBST(MAGIC_LIBS)
AM_CONDITIONAL(ENABLE_MAGIC, test "x$enable_magic" = xyes)





