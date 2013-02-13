#include "mas_client_def.h"
#include "mas_basic_def.h"

#include <stdlib.h>

#include <mastar/wrap/mas_lib.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifdef MAS_CLIENT_LOG
#  include <mastar/log/mas_logger.h>
#endif
#include "init/inc/mas_opts.h"
#include "mas_client_readline.h"

#include "mas_init_client.h"

/*
this:
  mas_init_client.c
related:
  mas_init.c
  mas_init_server.c
  mas_init_threads.c
more:
  mas_init_client.h
  mas_init.h
  mas_init_server.h
  mas_init_threads.h
*/


int
mas_init_client( void ( *atexit_fun ) ( void ), int initsig, int argc, char **argv, char **env )
{
  int r = 0;

  HMSG( "INIT CLIENT" );
#ifndef MAS_CLIENT_LOG
  ctrl.log_disabled = 1;
#endif
  /* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server */
  ctrl.is_client = 1;
  ctrl.is_server = 0;
  r = mas_pre_init( argc, argv, env );

  if ( r >= 0 )
  {
    r = mas_opts_restore_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL );
    if ( r <= 0 )
      r = mas_opts_restore( NULL, ctrl.progname ? ctrl.progname : "Unknown" );
  }
  HMSG( "-> INIT" );
  if ( r >= 0 )
    r = mas_init( atexit_fun, initsig, argc, argv, env );
  HMSG( "<- INIT" );
  if ( r >= 0 )
    r = mas_client_init_readline(  );
  if ( r >= 0 )
  r = mas_post_init( argc, argv, env );
  return r;
}

void
mas_destroy_client( void )
{
#ifdef MAS_CLIENT_LOG
  /* {                          */
  mas_logger_cleanup( NULL );
  /*   mas_logger_close(  );    */
  /*   mas_log_clean_queue(  ); */
  /* }                          */
#endif
  if ( !ctrl.opts_saved )
    mas_opts_save( NULL, ctrl.progname ? ctrl.progname : "Unknown" );
  {
    if ( !ctrl.opts_saved_plus )
      mas_opts_save_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL );
  }

  mas_client_destroy_readline(  );
  /* if ( ctrl.restart )                                                                  */
  /* {                                                                                    */
  /*   HMSG( "restart sleep %10.5f sec and run %s", ctrl.restart_sleep, opts.argv[0] ); */
  /*   mas_nanosleep( ctrl.restart_sleep );                                               */
  /* }                                                                                    */
  mas_destroy(  );
}
