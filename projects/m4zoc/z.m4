dnl remade from gthumb-3.4.0 configure.ac
AC_ARG_ENABLE([z],
	      [AS_HELP_STRING([--disable-z],[do not compile code that uses the zlib library])],,
	      [enable_z=yes])
if test "x$enable_z" = "xyes"; then
	SAVE_CFLAGS="$CFLAGS"
	CFLAGS=""
	AC_CHECK_LIB(z, crc32,
        	     [enable_z=yes],
	     	     [enable_z=no])
	if test "x$enable_z" = "xyes"; then
		AC_DEFINE(HAVE_LIBZ, 1, [Define to 1 if zlib support is included])
		Z_LIBS='-lz -lm'
	fi
	CFLAGS="$SAVE_CFLAGS"
	unset SAVE_CFLAGS
fi
AC_SUBST(Z_LIBS)
AM_CONDITIONAL(ENABLE_Z, test "x$enable_z" = xyes)





