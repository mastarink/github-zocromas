SUBDIRS= inc
bin_PROGRAMS=zocromas_runonce

suidperms = 4755
sgidperms = 2755


zocromas_runonce_SOURCES= mas_runonce.c mas_runonce_base.c mas_runonce_cli.c mas_runonce_scan.c mas_runonce_action.c mas_runonce_pid.c mas_runonce_section_pid.c mas_runonce_test.c mas_runonce_config.c mas_runonce_config_group.c mas_runonce_config_section.c mas_runonce_window.c
# zocromas_runonce_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ -lcap
zocromas_runonce_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ @GLIB_CFLAGS@
# zocromas_runonce_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@
zocromas_runonce_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ @GLIB_LIBS@ -lX11 -lXmu
# zocromas_runonce_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ -rdynamic


if MAS_TEST
noinst_PROGRAMS=wmctrl

wmctrl_LDFLAGS=@AM_LDFLAGS@  @GLIB_LIBS@ -lX11 -lXmu
wmctrl_CFLAGS=@GLIB_CFLAGS@
wmctrl_SOURCES=wmctrl.c
else

endif

install-exec-hook:
	chmod ug+s $(DESTDIR)$(bindir)/@PACKAGE_NAME@

##install-am: all-am
##        $(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
##        for i in $(suidbins); do \
##      	  chmod -f $(suidperms) $(DESTDIR)$(bindir)/$$i; \
##        done

