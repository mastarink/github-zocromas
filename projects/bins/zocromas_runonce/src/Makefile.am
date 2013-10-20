SUBDIRS= inc
bin_PROGRAMS=zocromas_runonce

suidperms = 4755
sgidperms = 2755


zocromas_runonce_SOURCES= mas_runonce.c mas_runonce_base.c mas_runonce_pid.c mas_runonce_test.c mas_runonce_config.c mas_runonce_config_group.c mas_runonce_config_section.c
# zocromas_runonce_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@ -lcap
zocromas_runonce_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@
# zocromas_runonce_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@
zocromas_runonce_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@
# zocromas_runonce_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ -rdynamic


install-exec-hook:
	chmod ug+s $(DESTDIR)$(bindir)/@PACKAGE_NAME@

##install-am: all-am
##        $(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
##        for i in $(suidbins); do \
##      	  chmod -f $(suidperms) $(DESTDIR)$(bindir)/$$i; \
##        done

