# AM_CFLAGS="${AM_CFLAGS} -pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror missing-field-initializers -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=\\\"${sysconfdir}\\\" -DMAS_LIBDIR=\\\"${libdir}\\\" "

# AM_CFLAGS="${AM_CFLAGS} -pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=${sysconfdir} -DMAS_LIBDIR=${libdir} "


# 20151005: 
AM_CFLAGS="${AM_CFLAGS}  -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Wextra -Wno-override-init -Wno-missing-field-initializers -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=\\\"${sysconfdir}\\\" -DMAS_LIBDIR=\\\"${libdir}\\\" "
# AM_CFLAGS="${AM_CFLAGS}  -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=${sysconfdir} -DMAS_LIBDIR=${libdir} "

AC_SUBST(AM_CFLAGS)
