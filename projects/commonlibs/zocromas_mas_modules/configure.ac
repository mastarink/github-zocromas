AC_PREREQ([2.67])
AC_INIT(m4_esyscmd([shn/prjname.sh]),m4_esyscmd([shn/zocversion.sh]),[bug-report@mastarink.net])

AC_CONFIG_SRCDIR([src/mas_modules_commands.c])
AC_CONFIG_AUX_DIR([.auxdir])
AC_CONFIG_MACRO_DIR([.auxdir/m4])


m4_include([m4zoc/init.m4])

LT_INIT

m4_include([m4zoc/vars0.m4])
m4_include([m4zoc/varscheck.m4])

# Checks for libraries.
PKG_CHECK_MODULES([DEPS], [zocromas_zoctypes zocromas_mas_wrap zocromas_mas_control zocromas_mas_tools zocromas_mas_msg zocromas_mas_log \
	zocromas_mas_fileinfo])









m4_include([m4zoc/vars1.m4])

m4_include([m4zoc/am_ldflags.m4])

m4_include([m4zoc/vars2.m4])

m4_include([m4zoc/am_cflags.m4])

m4_include([m4zoc/vars3.m4])
m4_include([m4zoc/msg.m4])
m4_include([m4zoc/output_l.m4])
