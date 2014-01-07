#include <unistd.h>

#include <mastar/types/mas_control_types.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>
#include <mastar/log/mas_logger.h>

#include <mastar/thtools/mas_thread_tools.h>

#include <mastar/listener/mas_listeners.h>

#include "mas_ticker.h"
#include "mas_watcher.h"


#include "mas_master_core.h"

/*
this:
  mas_master.c
related:
  mas_server_main.c
  mas_listener.c
  mas_listener_wait_client.c
*/

/*
 * 1. optionally start service threads
 *   1.1. logger
 *   1.2. ticker
 *   1.3. watcher
 * 2. run
 * 3. stop service threads
 *   3.1. logger
 *   3.2. ticker
 *   3.3. watcher
 * */

int
mas_master_core( const mas_options_t * popts )
{
  CTRL_PREPARE;
  int r = 0, rn = 0;
  mas_control_t *this = &ctrl;

  MSTAGE( CORE );

  HMSG( "MASTER START : pid:%u %c%c%c", getpid(  ), OPT_QFLAG( popts, log_run ) ? 'L' : 'l', !OPT_QFLAG( popts, woticker ) ? 'T' : 't',
        !OPT_QFLAG( popts, wowatcher ) ? 'W' : 'w' );
  MAS_LOG( "to start spec. threads" );
  if ( OPT_QFLAG( popts, log_run ) )
  {
    HMSG( "LOGGER TO START" );
    mas_logger_start(  );
  }
  else
  {
    HMSG( "NO LOGGER" );
  }
  if ( !OPT_QFLAG( popts, woticker ) )
  {
    HMSG( "TICKER TO START" );
    mas_ticker_start( popts );
  }
  else
  {
    MAS_LOG( "running w/o ticker" );
    HMSG( "NO TICKER" );
  }
  if ( OPT_QFLAG( popts, wowatcher ) )
  {
    HMSG( "WATCHER TO START" );
    mas_watcher_start( popts );
  }
  else
  {
    MAS_LOG( "running w/o watcher" );
    HMSG( "NO WATCHER" );
  }
  if ( popts->womaster )
  {
    HMSG( "NO MASTER" );
    sleep( popts->womaster );
  }
  else if ( popts->wolistener )
  {
    MAS_LOG( "stopped run w/o listeners" );
    sleep( popts->wolistener );
  }
  else
  {
    if ( popts->hostsv.c > 0 && popts->hostsv.v )
    {
      HMSG( "TO MASTER LOOP r:%d; ctrl.keep_listening:%d; ctrl.fatal:%d; flag-quit:%d", r, ctrl.keep_listening, ctrl.fatal,
            OPT_QFLAG( popts, quit ) );
      while ( r >= 0 && ctrl.keep_listening && !ctrl.fatal && !OPT_QFLAG( popts, quit ) )
        r = mas_listeners_main( popts );
      thMSG( "just out of server main loop" );
      MAS_LOG( "just out of server main loop" );
    }
    else
    {
      HMSG( "MASTER C.3" );
      EMSG( "hosts not defined" );
      MAS_LOG( "hosts not defined" );
    }
  }
  HMSG( "MASTER D" );
  if ( popts->exitsleep )
    sleep( popts->exitsleep );
  MAS_LOG( "to stop spec. threads" );
  WMSG( "TO STOP LOGGER" );
  mas_logger_stop(  );
  WMSG( "TO STOP TICKER" );
  mas_ticker_stop(  );
  MAS_LOG( "stopped spec. threads" );

  /* mas_watcher_stop(  ); */
  ctrl.watcher_stop = 1;

  MAS_LOG( "exiting master server" );
  HMSG( "MASTER TO END : %d", r );
  {
    EVAL_PREPARE;
/* Set the name of the calling thread, using the value in the location pointed to by (char *) arg2. The name can be up to 16 bytes long, and should be null-terminated if it contains fewer bytes. This is the same attribute that can be set via pthread_setname_np(3) and retrieved using pthread_getname_np(3). The attribute is likewise accessible via /proc/self/task/[tid]/comm, where tid is the name of the calling thread. */
    IEVAL( rn, mas_thself_set_name( ctrl.is_parent ? popts->thname.parent_masterx : popts->thname.daemon_masterx, "zocMasterX" ) );
  }
  return r;
}
