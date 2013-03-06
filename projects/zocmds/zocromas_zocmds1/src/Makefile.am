SUBDIRS= inc
##########################################################################################
######### modules ########################################################################
##########################################################################################

moduledir=$(libdir)/zocmds
module_LTLIBRARIES = \
	check.la \
	client.la \
	ctrl.la \
	do.la \
	get.la \
	listener.la \
	mem.la \
	readline.la \
	server.la \
	set.la \
	show.la \
	root.la

check_la_LDFLAGS         =    -module -avoid-version -no-undefined @AM_LDFLAGS@
check_la_SOURCES         = mas_cmdmod_check.c
                       
client_la_LDFLAGS        = -module -avoid-version -no-undefined @AM_LDFLAGS@
client_la_SOURCES        = mas_cmdmod_client.c
                       
ctrl_la_LDFLAGS          =    -module -avoid-version -no-undefined @AM_LDFLAGS@
ctrl_la_SOURCES          = mas_cmdmod_ctrl.c
                       
do_la_LDFLAGS            =    -module -avoid-version -no-undefined @AM_LDFLAGS@
do_la_SOURCES            = mas_cmdmod_do.c
                       
get_la_LDFLAGS           =    -module -avoid-version -no-undefined @AM_LDFLAGS@
get_la_SOURCES           = mas_cmdmod_get.c
                       
listener_la_LDFLAGS      = -module -avoid-version -no-undefined @AM_LDFLAGS@
listener_la_SOURCES      = mas_cmdmod_listener.c
                       
mem_la_LDFLAGS           =    -module -avoid-version -no-undefined @AM_LDFLAGS@
mem_la_SOURCES           = mas_cmdmod_mem.c
                       
readline_la_LDFLAGS      =    -module -avoid-version -no-undefined @AM_LDFLAGS@
readline_la_SOURCES      = mas_cmdmod_readline.c
                       
server_la_LDFLAGS        = -module -avoid-version -no-undefined @AM_LDFLAGS@
server_la_SOURCES        = mas_cmdmod_server.c

set_la_LDFLAGS           =    -module -avoid-version -no-undefined @AM_LDFLAGS@
set_la_SOURCES           = mas_cmdmod_set.c
                      
show_la_LDFLAGS          =   -module -avoid-version -no-undefined @AM_LDFLAGS@
show_la_SOURCES          = mas_cmdmod_show.c



root_la_LDFLAGS          =    -module -avoid-version -no-undefined @AM_LDFLAGS@
root_la_SOURCES          = mas_cmdmod_root.c

##########################################################################################

