SUBDIRS= inc client server
##########################################################################################
## cd /home/mastar/.mas/develop/autotools/zocromas
## export MAS_ZOCROMAS_DEFOPTS='-H193.222.140.165:5002 -H193.222.140.165:5003 -L/mnt/miscn/mastar/develop/autotools/zocromas/log'
##
## sh/run_server.sh
##
## sh/run_client.sh
##
## /usr/sbin/ab2 -c3000 -n100000 http://192.168.71.2:5001/
##
## lwp-request  http://mastarink.net:6003/
##
## libtool --mode=execute gdb ./src/.libs/mas_server
##########################################################################################

###  MAKEFLAGS=-s -j3 ###

##########################################################################################
############ libraries ###################################################################
##########################################################################################

##########################################################################################
############# binaries ###################################################################
##########################################################################################

# bin_PROGRAMS=mas_minitest
# mas_parg



##########################################################################################

# mas_parg_SOURCES=parg.c

# mas_minitest_SOURCES=minitest.c
# mas_minitest_CFLAGS=${AM_CFLAGS} -L\$(prefix)/lib -lmas_wrap -lmas_arg_tools -lmas_control
# mas_minitest_CFLAGS=

##########################################################################################
##########################################################################################

# ( cd src && gcc -pedantic  -Wall -Werror -include ../config.h -fpic -std=gnu99 -D_GNU_SOURCE -D_XOPEN_SOURCE=600 mas_client.c mas_io.c mas_open.c mas_cs.c mas_sig.c  -lreadline -Wl,--wrap=malloc,--wrap=free,--wrap=realloc,--defsym,__MAS_LINK_DATE__="`date +0x%Y%m%d`",--defsym,__MAS_LINK_TIME__="`date +0x%H%M%S`",--defsym,__MAS_LINK_TIMESTAMP__="`date +%s`" -o mas_client )

# small binary, w/o debug:
# ./configure CFLAGS=-Os ; make


# bin_PROGRAMS = gui-program cli-program
# gui_program_SOURCES = gui.c
# cli_program_SOURCES = cli.c

## Only our GUI program uses GTK+ at the moment.
#template_CPPFLAGS = @GTK_CFLAGS@ @GLADE_CFLAGS@ @GMODULE_CFLAGS@
#template_LDADD = @GTK_LIBS@ @GLADE_LIBS@ @GMODULE_LIBS@
#   template_LDFLAGS = -Wl,--export-dynamic

