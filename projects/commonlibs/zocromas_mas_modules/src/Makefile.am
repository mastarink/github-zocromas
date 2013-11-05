SUBDIRS= inc

lib_LTLIBRARIES = libzocromas_mas_modules.la

libzocromas_mas_modules_la_SOURCES = mas_modules_commands.c mas_modules_commands_eval.c mas_modules_load_module.c  mas_modules_ctrl_module.c
# -ldl
libzocromas_mas_modules_la_LDFLAGS = @AM_LDFLAGS@ -ldl @DEPS_LIBS@
