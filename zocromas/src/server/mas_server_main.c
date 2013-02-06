#include "mas_server_def.h"
#include "mas_basic_def.h"

/**
 * cd /home/mastar/.mas/develop/autotools/zocromas
 * export MAS_ZOCROMAS_DEFOPTS='-H193.222.140.165:5002 -H193.222.140.165:5003 -L/mnt/miscn/mastar/develop/autotools/zocromas/log'
 *
 * sh/run_server.sh
 *
 * sh/run_client.sh
 *
 * /usr/sbin/ab2 -c3000 -n100000 http://192.168.71.2:5001/
 *
 * lwp-request  http://mastarink.net:6003/
 *
 * libtool --mode=execute gdb ./src/.libs/mas_server 
 *
 * see sh/debug_*.sh
 *
 * see sh/run_*.sh
 * 
 * */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <mastar/wrap/mas_lib_thread.h>

/* #include "mas_common.h" */

#include "mas_master.h"


/*
this:
  mas_server_main.c
related:
  mas_master.c
  mas_curses.c
  mas_listener_wait_client.c
  mas_server_sig.c
  mas_client_main.c

  mas_control.c
  mas_opts.c

*/

/* (gdb) set print thread-events off    */
/* (gdb) handle SIGINT nostop noprint   */
/* (gdb) handle SIGPIPE nostop noprint  */
/* (gdb) run -P5001 -H127.0.0.1 --nomsg */

/* SEGFAULT:                                         */
/* -------- */
/* ulimit -c unlimited */
/* -------- */
/* /etc/sysctl.conf -->                              */
/* -------- */
/* kernel.core_uses_pid = 1                          */
/* kernel.core_pattern = /tmp/core-%e-%s-%u-%g-%p-%t */
/* fs.suid_dumpable = 2                              */
/* -------- */
/* sysctl -p */
/* -------- */
/* gdb /path/to/application /path/to/corefile */
/* libtool --mode=execute gdb ./src/.libs/mas_server ./core-mas_server-11-1000-1000-10500-135 */


/* in gdb:
 *   info threads
 *   ** to switch 'current' thread:
 *   thread 2
 * */

/* /usr/sbin/ab2 -r  -t20 -c30 -n100000 http://127.0.0.1:5001/       */
/* /usr/bin/siege -lsiege.log -b -c100 -r1000 http://127.0.0.1:5001/ */





/* error codes */
/* - /usr/include/errno.h                  */
/* + /usr/include/bits/errno.h             */
/*   /usr/include/features.h               */
/* - /usr/include/linux/errno.h            */
/* - /usr/include/asm/errno.h              */
/* + /usr/include/asm-generic/errno.h      */
/* + /usr/include/asm-generic/errno-base.h */


int
main( int argc, char *argv[], char *env[] )
{
  int r = 0;

  r = mas_master_bunch( argc, argv, env );
  mas_pthread_exit( &r );
  return r;
}
