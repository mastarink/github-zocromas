#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <uuid/uuid.h>
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

#include <mastar/modules/mas_modules_ctrl_module.h>

/* mas_listeners_stop */
#include <mastar/listener/mas_listener_control_list.h>
#include <mastar/listener/mas_listeners.h>

/* mas_in_thread_end */
#include <mastar/thtools/mas_thread_tools.h>

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


/* __attribute__ ( ( constructor ) )                                                    */
/*      static void master_constructor( void )                                          */
/* {                                                                                    */
/*   if ( stderr )                                                                      */
/*     fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno ); */
/* }                                                                                    */



/* #ifdef MAS_INIT_SEPARATE                                                                          */
/* int                                                                                               */
/* mas_init_server( void ( *atexit_fun ) ( void ), int initsig, int argc, char **argv, char **env )  */
/* {                                                                                                 */
/*   int r = 0;                                                                                      */
/*                                                                                                   */
/*   ctrl.status = MAS_STATUS_START;                                                                 */
/*   ctrl.start_time = mas_double_time(  );                                                          */
/*                                                                                                   */
/*                                                                                                   */
/* #  ifdef MAS_SERVER_NOLOG                                                                         */
/*   ctrl.log_disabled = 1;                                                                          */
/* #  endif                                                                                          */
/*   (* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server *) */
/*   ctrl.is_client = 0;                                                                             */
/*   ctrl.is_server = 1;                                                                             */
/*   (* r = mas_pre_init( argc, argv, env ); *)                                                      */
/*   IEVAL( r, mas_pre_init( argc, argv, env ) );                                                    */
/*                                                                                                   */
/*   MAS_LOG( "init server" );                                                                       */
/* #  ifdef MAS_USE_CURSES                                                                           */
/*   (* if ( r >= 0 )              *)                                                                */
/*   (*   r = mas_init_curses(  ); *)                                                                */
/* #  endif                                                                                          */
/*   (* if ( r >= 0 )                                           *)                                   */
/*   (*   r = mas_init( atexit_fun, initsig, argc, argv, env ); *)                                   */
/*   IEVAL( r, mas_init( atexit_fun, initsig, argc, argv, env ) );                                   */
/*   (* if ( r >= 0 )              *)                                                                */
/*   (*   r = mas_init_daemon(  ); *)                                                                */
/*   IEVAL( r, mas_init_daemon(  ) );                                                                */
/*   (* malloc_trim( 0 ); *)                                                                         */
/*   (* if ( ctrl.is_parent )       *)                                                               */
/*   (* {                           *)                                                               */
/*   (*   HMSG( "PARENT to exit" ); *)                                                               */
/*   (* }                           *)                                                               */
/*   (* else                        *)                                                               */
/*   {                                                                                               */
/*     MAS_LOG( "(%d) init server: to init threads", r );                                            */
/*     (* if ( r >= 0 )               *)                                                             */
/*     (*   r = mas_threads_init(  ); *)                                                             */
/*     IEVAL( r, mas_threads_init(  ) );                                                             */
/*     MAS_LOG( "(%d) init server: to load protos", r );                                             */
/*     (* if ( r >= 0 )                   *)                                                         */
/*     (*   r = mas_init_load_protos( mas_options_t * popts ); *)                                   */
/*     IEVAL( r, mas_init_load_protos( mas_options_t * popts ) );                                   */
/*     MAS_LOG( "(%d) init server: to create lcontrols", r );                                        */
/*     if ( r >= 0 )                                                                                 */
/*       mas_lcontrols_list_create(  );                                                              */
/*     MAS_LOG( "init server done" );                                                                */
/*     MAS_LOG( "(%d) init server: to post-init", r );                                               */
/*     (* if ( r >= 0 )            *)                                                                */
/*     (*   r = mas_post_init(  ); *)                                                                */
/*     IEVAL( r, mas_post_init(  ) );                                                                */
/*     MAS_LOG( "(%d) end init server", r );                                                         */
/*   }                                                                                               */
/*   return r;                                                                                       */
/* }                                                                                                 */
/* #endif                                                                                            */
void
mas_destroy_server( mas_options_t * popts )
{
  extern mas_control_t ctrl;

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
  mas_modules_unregister(  );

  mas_destroy_pids( popts );
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
  IMSG( "logQsize:%lu - %lu = %lu", ctrl.log_q_came, ctrl.log_q_gone, ctrl.log_q_came - ctrl.log_q_gone );
}
