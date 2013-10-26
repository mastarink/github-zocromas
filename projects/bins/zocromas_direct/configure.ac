#( cd /.... ; rm -Rf build ; mkdir build ; rm -Rf mascliser-* ; tar -jxf ../piptest/mascliser-1.0.1.tar.bz2 ; cd build ; ../mascliser-*/configure ; make ; l src ) ; cd .
# find $MAS_PROJECTS_DIR -name 'configure.ac' -exec gvim -fd \{} ./commonlibs/zocromas_mas_maslibtemplate/configure.ac  \;


#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])

# m4_define([R_VERSION],
#           m4_bpatsubst(m4_esyscmd([cat VERSION]),
#       		 [\([0-9.]*\)\(\w\|\W\)*],
#       		 [\1]))


# If your `configure.in' has:
#

# AC_INIT
# AC_CONFIG_SRCDIR([src/foo.c])
# AM_INIT_AUTOMAKE(mumble, 1.5)

# you can modernize it as follow:
#

# AC_CONFIG_SRCDIR(src/foo.c)
# AM_INIT_AUTOMAKE

# AC_INIT([mas_maslibtemplate],[1.0.1.3],[bug-report@mastarink.net])
AC_INIT(m4_esyscmd([shn/prjname.sh]),m4_esyscmd([shn/zocversion.sh]),[bug-report@mastarink.net])
#AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_SRCDIR([src/mas_direct.c])
AC_CONFIG_AUX_DIR([.auxdir])
AC_CONFIG_MACRO_DIR([.auxdir/m4])
AM_PROG_AR
AM_INIT_AUTOMAKE([foreign  silent-rules -Wall -Werror dist-bzip2])
# AM_MAINTAINER_MODE([enable])

# AM_EXTRA_RECURSIVE_TARGETS([foo])
AM_SILENT_RULES([yes])


# ?? AM_MAINTAINER_MODE([enable])

# AM_INIT_AUTOMAKE([foreign               -Wall -Werror dist-bzip2 nostdinc])



# Checks for programs.
# AC_PROG_GCC_DEFAULT_FLAGS([-O0 -Wall])

# To turn off '-g -O2' :
# ${CFLAGS="-O2"}
# # CFLAGS="-Os"
# AC_SUBST([CFLAGS])

# ${CFLAGS="-O0 -g -ggdb"}
CFLAGS=""
AC_SUBST([CFLAGS])
####################### tmp ? off 20130606.094428
############ CFLAGS="-O0 -g -ggdb"
###########  AC_SUBST([CFLAGS])


AC_PROG_CC
AC_PROG_CXX

# AM_PROG_CC_STDC
# AC_C_CONST
AM_PROG_CC_C_O

LT_INIT
AC_PROG_INSTALL

AC_HEADER_STDC
# AM_PATH_GTK_2_0([2.10.0],,AC_MSG_ERROR([Gtk+ 2.10.0 or higher required.]))
AC_LANG([C])

AC_CHECK_FUNCS([memset dup2 atexit  mkfifo socket strchr strdup strndup strtol strstr inet_ntoa gethostbyname gettimeofday])
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h malloc.h netdb.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_STRERROR_R
# AC_PROG_RANLIB --- libtoolize: `AC_PROG_RANLIB' is rendered obsolete by `LT_INIT'
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T


# Checks for libraries.
# AC_CHECK_LIB([pthread], [pthread_create])
# GLADE_LIBS="`pkg-config --libs libglade-2.0`"
# AC_SUBST(GLADE_LIBS)
# GLADE_CFLAGS="`pkg-config --cflags libglade-2.0`"
# AC_SUBST(GLADE_CFLAGS)

# PKG_CHECK_MODULES([GLADE], [libglade-2.0])
# PKG_CHECK_MODULES([GMODULE], [gmodule-2.0])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

PKG_CHECK_MODULES([DEPS], [zocromas_zoctypes zocromas_mas_wrap zocromas_mas_tools zocromas_mas_msg zocromas_mas_fileinfo zocromas_mas_varset zocromas_mas_init zocromas_mas_modules])



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



# AM_LDFLAGS="-Wl,--export-dynamic"
# AM_LDFLAGS="-ldl -Wl,--wrap=strdup,--wrap=malloc,--wrap=free,--wrap=realloc,--defsym,__MAS_LINK_DATE__=\`date +0x%Y%m%d\`,--defsym,__MAS_LINK_TIME__=\`date +0x%H%M%S\`,--defsym,__MAS_LINK_TIMESTAMP__=\`date +%s\`"
### 64bit:  ,--defsym,__MAS_LINK_DATET__=\`date +0x%Y%m%d%H%M%S\`
AM_LDFLAGS="-Wl,--defsym,__MAS_LINK_DATE__=\`date +0x%Y%m%d\`,--defsym,__MAS_LINK_TIME__=\`date +0x%H%M%S\`,--defsym,__MAS_LINK_TIMESTAMP__=\`date +%s\`"
# __MAS_LINK_DATE__=\`date +0x%Y%m%d\`
AC_SUBST(AM_LDFLAGS)


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

AM_CFLAGS="${AM_CFLAGS} -pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR='${sysconfdir}'"
# AM_CFLAGS="-pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -Wall -Werror -include config.h -pipe"
AC_SUBST(AM_CFLAGS)

AC_DEFINE_UNQUOTED(MAS_CFLAGS, "${AM_CFLAGS}", [compile flags])
AC_SUBST(MAS_CFLAGS)


# AM_CXXFLAGS="-O0"
# AC_SUBST(AM_CXXFLAGS)

AM_CPPFLAGS="-I\$(srcdir)/inc -I\$(top_srcdir)/src/inc -I\$(top_srcdir)/src -I\$(includedir)"
AC_SUBST(AM_CPPFLAGS)

AH_TEMPLATE([MAS_C_DATE],[mas_c_date])
AC_DEFINE_UNQUOTED( MAS_C_DATE, "`date +%Y%m%d%H%M%S`", [cdate])
AC_SUBST([MAS_C_DATE])

AH_TEMPLATE([MAS_MALLOC])
AC_DEFINE( MAS_MALLOC, malloc)
AC_SUBST([MAS_MALLOC])

AH_TEMPLATE([MAS_FREE])
AC_DEFINE( MAS_FREE, free)
AC_SUBST([MAS_FREE])

AH_TEMPLATE([MAS_CALLOC])
AC_DEFINE( MAS_CALLOC, calloc)
AC_SUBST([MAS_CALLOC])

AH_TEMPLATE([MAS_STRDUP])
AC_DEFINE( MAS_STRDUP, strdup)
AC_SUBST([MAS_STRDUP])






AH_TEMPLATE([MAS_UNAME],[mas_uname])
AC_DEFINE_UNQUOTED( MAS_UNAME, "`uname -a`", [uname])
AC_SUBST([MAS_UNAME])

AH_TEMPLATE([MAS_OSVER],[mas_osver])
AC_DEFINE_UNQUOTED( MAS_OSVER, "`cat /proc/version`", [os version])
AC_SUBST([MAS_OSVER])

AH_TEMPLATE([MAS_UUID],[mas_uuid])
AC_DEFINE_UNQUOTED( MAS_UUID, "`uuidgen`", [uuidgen])
AC_SUBST([MAS_UUID])

AH_TEMPLATE([MAS_S_DATE],[mas_s_date])
AC_DEFINE_UNQUOTED( MAS_S_DATE, "`date +%s`", [sdate])
AC_SUBST([MAS_S_DATE])

#OBJDIR=masobj
AC_MSG_NOTICE([@@@@@@@@@ DEPS_LIBS: ${DEPS_LIBS} @@@@@@@@@])
AC_MSG_NOTICE([@@@@@@@@@ DEPS_CFLAGS: ${DEPS_CFLAGS} @@@@@@@@@])


AC_CONFIG_HEADERS([config.h])
# AC_CONFIG_COMMANDS([zocrromas_etc], [date > zzzz])
# AC_CONFIG_FILES([etc/.........])

AC_CONFIG_FILES([
		Makefile
		src/Makefile
		src/inc/Makefile

		])
AC_OUTPUT

