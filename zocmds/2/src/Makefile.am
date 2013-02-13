SUBDIRS= inc
##########################################################################################
######### modules ########################################################################
##########################################################################################

pkglibdir=$(libdir)/zocmds
pkglib_LTLIBRARIES = \
	notify.la

                      
notify_la_SOURCES        = mas_cmdmod2_notify.c
notify_la_LDFLAGS        =  @AM_LDFLAGS@  @LIBNOTIFY_LIBS@ @GTK_LIBS@   -module -avoid-version 
notify_la_CFLAGS        =   @AM_CFLAGS@ @LIBNOTIFY_CFLAGS@  @GTK_CFLAGS@
                       
##########################################################################################

