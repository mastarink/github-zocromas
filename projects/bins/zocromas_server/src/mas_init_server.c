#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <unistd.h>
#include <stdlib.h>
/* #include <uuid/uuid.h> */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/prctl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifndef MAS_SERVER_NOLOG
#  include <mastar/log/mas_logger.h>
#  include <mastar/log/mas_log.h>
#endif

#include <mastar/types/mas_control_types.h>

#include <mastar/unimodule/mas_unimodule_ctrl.h>

/* mas_listeners_stop */
#include <mastar/listener/mas_listener_control_list.h>
#include <mastar/listener/mas_listeners.h>

/* mas_in_thread_end */
#include <mastar/thtools/mas_thread_tools.h>
#include <mastar/cliopts/mas_cliopts_data.h>

#include "mas_init_threads.h"
#include "mas_init_protos.h"

#include <mastar/init/mas_init.h>

#include "mas_ticker.h"

#include "mas_init_pid.h"
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
mas_server_destroy( mas_options_t * popts )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
  mas_control_t *this = &ctrl;

  MSTAGE( DESTROY );
  {
    int rn = 0;
    char name_buffer[32] = "?";

    IEVAL( rn, prctl( PR_GET_NAME, ( unsigned long ) name_buffer ) );
    MAS_LOG( "destroy server" );
    HMSG( "DESTROY SERVER %s; parent:%d - %lx : %lx", name_buffer, ctrl.is_parent, ctrl.threads.n.master.thread, mas_pthread_self(  ) );
  }
  {
    int rn = 0;

    if ( ctrl.is_parent )
    {
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParServerD" ) );
    }
    else
    {
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaeServerD" ) );
    }
  }
  MAS_LOG( "to save opts" );
  /* if ( !ctrl.opts_saved )                                                                                            */
  /*   mas_opts_save( NULL, ctrl.progname ? ctrl.progname : "Unknown" );                                                */
  /* if ( !ctrl.opts_saved_plus )                                                                                       */
  /*   mas_opts_save_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL ); */
  {
    int lstopped;

    lstopped = mas_listeners_stop(  );
    /* if ( lstopped ) */
    {
      HMSG( "STOPPED %d lsn's", lstopped );
    }
  }
  /* mas_channel_deaf( &ctrl, ctrl.pchannel ); */

#ifdef MAS_USE_CURSES
  if ( use_curses )
    sleep( 1 );
  mas_close_curses(  );
#endif
  mas_lcontrols_delete_list(  );
  mas_in_thread_end(  );

  mas_threads_destroy(  );
  /* mas_threads_destroy( popts ); */

  mas_protos_destroy(  );


  mas_destroy( popts );
  MAS_LOG( "to cancel ticker" );
  MAS_LOG( "to cancel logger" );
  if ( ctrl.threads.n.logger.thread )
  {
    EMSG( "TO STOP LOGGER" );
    mas_logger_stop(  );
  }
  if ( ctrl.threads.n.ticker.thread )
  {
    EMSG( "TO STOP TICKER" );
    mas_ticker_stop(  );
  }
  HMSG( "TO DESTROY MODULES" );
  mas_unimodule_unregister(  );

  mas_pids_destroy( popts );
  mas_cliopts_data_destroy( popts );
  /* {                                                      */
  /*   if ( ctrl.pidfd > 0 )                                */
  /*   {                                                    */
  /*     (* char *pidpath;                           *)     */
  /*     (*                                          *)     */
  /*     (* pidpath = mas_strdup( popts-> dir.pids );    *) */
  /*     (* pidpath = mas_strcat_x( pidpath, name ); *)     */
  /*                                                        */
  /*     mas_close( ctrl.pidfd );                           */
  /*     (* unlink( pidpath ); *)                           */
  /*   }                                                    */
  /*   ctrl.pidfd = 0;                                      */
  /* }                                                      */

  MAS_LOG( "destroy server done" );
  HMSG( "DESTROY SERVER DONE" );
  /* IMSG( "logQsize:%lu - %lu = %lu", ctrl.log_q_came, ctrl.log_q_gone, ctrl.log_q_came - ctrl.log_q_gone ); */
}

__attribute__ ( ( constructor( 1001 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_constructor( IL, 0 );
}
