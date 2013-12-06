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
AC_CONFIG_SRCDIR([src/mas_runonce.c])

m4_include([m4zoc/vars0.m4])
m4_include([m4zoc/varscheck.m4])


# Checks for libraries.

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.3])
AC_SUBST(GLIB_CFLAGS)

PKG_CHECK_MODULES([DEPS], [zocromas_mas_tools])


m4_include([m4zoc/vars1.m4])




# AM_LDFLAGS="-Wl,--export-dynamic"
# AM_LDFLAGS="-ldl -Wl,--wrap=strdup,--wrap=malloc,--wrap=free,--wrap=realloc,--defsym,__MAS_LINK_DATE__=\`date +0x%Y%m%d\`,--defsym,__MAS_LINK_TIME__=\`date +0x%H%M%S\`,--defsym,__MAS_LINK_TIMESTAMP__=\`date +%s\`"
### 64bit:  ,--defsym,__MAS_LINK_DATET__=\`date +0x%Y%m%d%H%M%S\`
AM_LDFLAGS="-Wl,--defsym,__MAS_LINK_DATE__=\`date +0x%Y%m%d\`,--defsym,__MAS_LINK_TIME__=\`date +0x%H%M%S\`,--defsym,__MAS_LINK_TIMESTAMP__=\`date +%s\`"
# __MAS_LINK_DATE__=\`date +0x%Y%m%d\`
AC_SUBST(AM_LDFLAGS)



m4_include([m4zoc/vars2.m4])


AM_CFLAGS="${AM_CFLAGS} -pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -fpic -Wall -Werror -include config.h -pipe ${DEPS_CFLAGS} -DMAS_SYSCONFDIR=\\\"${sysconfdir}\\\" -DMAS_LIBDIR=\\\"${libdir}\\\""
# AM_CFLAGS="-pedantic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -Wall -Werror -include config.h -pipe"
AC_SUBST(AM_CFLAGS)

m4_include([m4zoc/vars3.m4])




#OBJDIR=masobj
AC_MSG_NOTICE([@@@@@@@@@ DEPS_LIBS: ${DEPS_LIBS} @@@@@@@@@])
AC_MSG_NOTICE([@@@@@@@@@ DEPS_CFLAGS: ${DEPS_CFLAGS} @@@@@@@@@])



m4_include([m4zoc/vars4.m4])

