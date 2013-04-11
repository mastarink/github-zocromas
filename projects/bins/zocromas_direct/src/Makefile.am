SUBDIRS= inc
bin_PROGRAMS=zocromas_direct

zocromas_direct_SOURCES= mas_direct.c mas_direct_sig.c
zocromas_direct_CFLAGS=@AM_CFLAGS@ -pthread @DEPS_CFLAGS@
zocromas_direct_LDFLAGS=@AM_LDFLAGS@ @DEPS_LIBS@ -rdynamic
