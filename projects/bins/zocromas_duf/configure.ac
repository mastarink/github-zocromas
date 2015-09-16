AC_PREREQ([2.67])
AC_INIT(m4_esyscmd([shn/prjname.sh]),m4_esyscmd([shn/zocversion.sh]),[bug-report@mastarink.net])

AC_CONFIG_SRCDIR([src/duf.c])
AC_CONFIG_AUX_DIR([.auxdir])
AC_CONFIG_MACRO_DIR([.auxdir/m4])


m4_include([m4zoc/init.m4])

LT_INIT

m4_include([m4zoc/vars0.m4])
m4_include([m4zoc/varscheck.m4])

# Checks for libraries.
AX_LIB_CRYPTO([yes])
AX_CHECK_CRYPTO_LIB([MD5],[MD5])
# AX_CHECK_CRYPTO_LIB([BSD],[BSD])
AX_LIB_SQLITE3([3.8])

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= 2.3])
AC_SUBST(GLIB_CFLAGS)

PKG_CHECK_MODULES([DEPS], [zocromas_mas_tools zocromas_mas_sqlite])

AC_CHECK_LIB([readline], [readline])


m4_include([m4zoc/vars1.m4])

#############################################################################################


m4_include([m4zoc/am_ldflags.m4])

m4_include([m4zoc/vars2.m4])

m4_include([m4zoc/am_cflags.m4])

m4_include([m4zoc/vars3.m4])
m4_include([m4zoc/msg.m4])
m4_include([m4zoc/output.m4])
