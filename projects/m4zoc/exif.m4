dnl remade from gthumb-3.4.0 configure.ac
AC_ARG_ENABLE([exif],
	      [AS_HELP_STRING([--disable-exif],[do not compile code that uses the libexif library])],,
	      [enable_exif=yes])
if test "x$enable_exif" = "xyes"; then
	SAVE_CFLAGS="$CFLAGS"
	CFLAGS=""
	AC_CHECK_LIB(exif, exif_content_get_entry,
        	     [enable_exif=yes],
	     	     [enable_exif=no])
	if test "x$enable_exif" = "xyes"; then
		AC_DEFINE(HAVE_LIBEXIF, 1, [Define to 1 if libexif support is included])
		EXIF_LIBS='-lexif -lm'
	fi
	CFLAGS="$SAVE_CFLAGS"
	unset SAVE_CFLAGS
fi
AC_SUBST(EXIF_LIBS)
AM_CONDITIONAL(ENABLE_EXIF, test "x$enable_exif" = xyes)




