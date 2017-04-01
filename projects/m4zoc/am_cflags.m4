# AM_CFLAGS="${AM_CFLAGS} -pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror missing-field-initializers -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=\\\"${sysconfdir}\\\" -DMAS_LIBDIR=\\\"${libdir}\\\" "

# AM_CFLAGS="${AM_CFLAGS} -pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=${sysconfdir} -DMAS_LIBDIR=${libdir} "


# 20151005: 
AM_CFLAGS="${AM_CFLAGS}  -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Wextra -Wno-override-init -Wno-missing-field-initializers -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=\\\"${sysconfdir}/${PACKAGE_NAME}\\\" -DMAS_LIBDIR=\\\"${libdir}\\\" -DMAS_LIBEXECDIR=\\\"${libexecdir}\\\"  -DMAS_SHAREDSTATEDIR=\\\"${sharedstatedir}\\\" -DMAS_LOCALSTATEDIR=\\\"${localstatedir}\\\"  -DMAS_DATADIR=\\\"${datadir}\\\"  -DMAS_BINDIR=\\\"${bindir}\\\" -DMAS_SBINDIR=\\\"${sbindir}\\\"  -DMAS_DATAROOTDIR=\\\"${datarootdir}\\\"    -DMAS_SRCDIR=\\\"${srcdir}\\\"   -DMAS_PREFIX=\\\"${prefix}\\\"  -DMAS_EXEC_PREFIX=\\\"${exec_prefix}\\\" "
# 20170228.134702
# AM_CFLAGS="${AM_CFLAGS}  -pedantic -std=gnu11 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Wextra -Wno-override-init -Wno-missing-field-initializers -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=\\\"${sysconfdir}\\\" -DMAS_LIBDIR=\\\"${libdir}\\\" -DMAS_LIBEXECDIR=\\\"${libexecdir}\\\"  -DMAS_SHAREDSTATEDIR=\\\"${sharedstatedir}\\\" -DMAS_LOCALSTATEDIR=\\\"${localstatedir}\\\"  -DMAS_DATADIR=\\\"${datadir}\\\"  -DMAS_BINDIR=\\\"${bindir}\\\" -DMAS_SBINDIR=\\\"${sbindir}\\\"  -DMAS_DATAROOTDIR=\\\"${datarootdir}\\\"    -DMAS_SRCDIR=\\\"${srcdir}\\\"   -DMAS_PREFIX=\\\"${prefix}\\\"  -DMAS_EXEC_PREFIX=\\\"${exec_prefix}\\\" "



# AM_CFLAGS= ${AM_CFLAGS}  -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Wextra -Wno-override-init -Wno-missing-field-initializers -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR='"$(sysconfdir)"' -DMAS_LIBDIR='"$(libdir)"'
AM_CPPFLAGS= ${AM_CPPFLAGS}   -DMAS_1SYSCONFDIR='"$(sysconfdir)"' -DMAS_1LIBDIR='"$(libdir)"'

# AM_CFLAGS="${AM_CFLAGS}  -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=${sysconfdir} -DMAS_LIBDIR=${libdir} "

AC_SUBST(AM_CFLAGS)
