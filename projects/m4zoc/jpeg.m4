AC_ARG_ENABLE([jpeg],
	      [AS_HELP_STRING([--disable-jpeg],[do not compile code that uses the libjpeg library])],,
	      [enable_jpeg=yes])
if test "x$enable_jpeg" = "xyes"; then
	SAVE_CFLAGS="$CFLAGS"
	CFLAGS=""
	AC_CHECK_LIB(jpeg, jpeg_destroy_decompress,
        	     [enable_jpeg=yes],
	     	     [enable_jpeg=no])
	if test "x$enable_jpeg" = "xyes"; then
		AC_DEFINE(HAVE_LIBJPEG, 1, [Define to 1 if libjpeg support is included])
		JPEG_LIBS='-ljpeg -lm -lz'
	fi
	CFLAGS="$SAVE_CFLAGS"
	unset SAVE_CFLAGS
fi
AC_SUBST(JPEG_LIBS)
AM_CONDITIONAL(ENABLE_JPEG_TOOLS, test "x$enable_jpeg" = xyes)


