AC_CONFIG_HEADERS([config.h])
# AC_CONFIG_COMMANDS([zocrromas_etc], [date > zzzz])
# AC_CONFIG_FILES([etc/.........])

AC_CONFIG_FILES([${PACKAGE_NAME}.pc
		Makefile
		src/Makefile
		src/inc/Makefile
		])

AC_OUTPUT



