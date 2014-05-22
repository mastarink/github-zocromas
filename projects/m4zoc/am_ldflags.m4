AM_LDFLAGS="${AM_LDFLAGS} -Wl,--defsym,__MAS_LINK_DATE__=\`date +0x%Y%m%d\`,--defsym,__MAS_LINK_TIME__=\`date +0x%H%M%S\`,--defsym,__MAS_LINK_TIMESTAMP__=\`date +%s\` "
AC_SUBST(AM_LDFLAGS)

