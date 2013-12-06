# AM_CFLAGS="${AM_CFLAGS} -pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=\\\"${sysconfdir}\\\" -DMAS_LIBDIR=\\\"${libdir}\\\""
AM_CFLAGS="${AM_CFLAGS} -pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=${sysconfdir} -DMAS_LIBDIR=${libdir}"
AC_SUBST(AM_CFLAGS)
