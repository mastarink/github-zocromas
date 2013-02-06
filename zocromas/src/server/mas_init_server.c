#include "mas_server_def.h"
#include "mas_basic_def.h"

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <uuid/uuid.h>

#include <mastar/wrap/mas_lib_thread.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include "listener/inc/mas_listener_control_list.h"
#include "listener/inc/mas_listeners.h"

#include "init/inc/mas_opts.h"
#include "mas_ticker.h"

#include <mastar/msg/mas_curses.h>

#include <mastar/log/mas_logger.h>
#include <mastar/log/mas_log.h>

#include <mastar/thtools/mas_thread_tools.h>
#include "mas_init_threads.h"

#include "mas_init_server.h"

/*
this:
  mas_init_server.c
related:
  mas_init_client.c
  mas_init.c
  mas_init_threads.c
more:
  mas_init_client.h
  mas_init.h
  mas_init_server.h
  mas_init_threads.h


*/


void
mas_init_server( void ( *atexit_fun ) ( void ), int initsig, int argc, char **argv, char **env )
{
  int r = 0;
  int k;

  /* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server */
  ctrl.is_client = 0;
  ctrl.is_server = 1;
  mas_pre_init( argc, argv, env );

  MAS_LOG( "init server" );
  mMSG( "PPID : %u - %s", getppid(  ), getenv( "MAS_PID_AT_BASHRC" ) );
  k = mas_opts_restore_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL );
  if ( k <= 0 )
    mas_opts_restore( NULL, ctrl.progname ? ctrl.progname : "Unknown" );
  /* r = mas_init_curses(  ); */
  if ( r < 0 && use_curses )
  {
    mas_close_curses(  );
    fprintf( stderr, "ERROR curses %d\n", r );
    /* exit( 33 ); */
  }

  mas_threads_init(  );
  mas_init( atexit_fun, initsig, argc, argv, env );
  mas_lcontrols_list_create(  );
  MAS_LOG( "init server done" );
}

void
mas_destroy_server( void )
{
  MAS_LOG( "destroy server" );
  MAS_LOG( "to save opts" );
  if ( !ctrl.opts_saved )
    mas_opts_save( NULL, ctrl.progname ? ctrl.progname : "Unknown" );
  if ( !ctrl.opts_saved_plus )
    mas_opts_save_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL );

  if ( ctrl.lcontrols_list )
  {
    MAS_LOG( "to cancel listeners" );
    FMSG( "\nTO CANCEL LISTENERS" );
    mas_listeners_cancel(  );
  }
  /* mas_channel_deaf( &ctrl, ctrl.pchannel ); */

  if ( use_curses )
    sleep( 1 );
  mas_close_curses(  );

  mas_lcontrols_delete_list(  );
  mas_in_thread_end(  );
  mas_threads_destroy(  );
  mas_destroy(  );
  MAS_LOG( "to cancel ticker" );
  MAS_LOG( "to cancel logger" );
  FMSG( "\nTO STOP LOGGER" );
  mas_logger_stop(  );
  FMSG( "\nTO STOP TICKER" );
  mas_ticker_stop(  );
  MAS_LOG( "destroy server done" );
  FMSG( "\nDESTROY SERVER DONE" );
}
