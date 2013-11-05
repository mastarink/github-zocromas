# AM_PATH_GTK_2_0([2.10.0],,AC_MSG_ERROR([Gtk+ 2.10.0 or higher required.]))

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

