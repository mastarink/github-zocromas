#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifdef MAS_CLIENT_LOG
#  include <mastar/log/mas_logger.h>
#  include <mastar/log/mas_log.h>
#endif


#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>

#include <mastar/init/mas_init.h>
#include <mastar/options/mas_opts.h>
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



void
mas_destroy_client( mas_options_t * popts )
{
#ifdef MAS_CLIENT_LOG
  /* {                          */
  mas_logger_cleanup( NULL );
  /*   mas_logger_close(  );    */
  /*   mas_log_clean_queue(  ); */
  /* }                          */
#endif
  /* if ( !ctrl.opts_saved )                                                                                              */
  /*   mas_opts_save( NULL, ctrl.progname ? ctrl.progname : "Unknown" );                                                  */
  /* {                                                                                                                    */
  /*   if ( !ctrl.opts_saved_plus )                                                                                       */
  /*     mas_opts_save_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL ); */
  /* }                                                                                                                    */

  mas_client_destroy_readline( popts );
  /* if ( ctrl.restart )                                                                  */
  /* {                                                                                    */
  /*   HMSG( "restart sleep %10.5f sec and run %s", ctrl.restart_sleep, opts.argv[0] ); */
  /*   mas_nanosleep( ctrl.restart_sleep );                                               */
  /* }                                                                                    */
  mas_destroy( popts );
}
