########### fileinfo   ########################################################################
AC_ARG_ENABLE([fileinfo],
	      AC_HELP_STRING([--enable-fileinfo],
			     [use fileinfo]),,
	      [enable_fileinfo=no])

if test x$enable_fileinfo = xyes ; then
    AC_DEFINE(MAS_HTTP_USE_FILEINFO, 1,
	[Enable fileinfo])
PKG_CHECK_MODULES([HTTP_DEPS], [zocromas_mas_fileinfo zocromas_mas_varset])
fi
########### autoobject   ########################################################################
AC_ARG_ENABLE([autoobject],
	      AC_HELP_STRING([--enable-autoobject],
			     [use autoobject]),,
	      [enable_autoobject=no])

if test x$enable_autoobject = xyes ; then
    AC_DEFINE(MAS_HTTP_USE_AUTOOBJECT, 1,
	[Enable autoobject])
PKG_CHECK_MODULES([HTTP_DEPS], [zocromas_mas_autoobject zocromas_mas_autoset])
fi



