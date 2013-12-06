#include <unistd.h>
#include <pthread.h>

#include <mastar/types/mas_control_types.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>


#include <mastar/log/mas_log.h>
#include <mastar/log/mas_logger.h>

#include <mastar/thtools/mas_thread_tools.h>


#include "mas_master_core.h"
#include "mas_master_thread.h"
#include "mas_master.h"

/*
this:
  mas_master.c
related:
  mas_server_main.c
  mas_listener.c
  mas_listener_wait_client.c
more:
  mas_ticker.c
  mas_watcher.c
  mas_cmdmod_listener.c
  mas_listener_control.c
  mas_listener_control.h
  mas_listener_control_list.c
  mas_listener_control_list.h
  mas_listener_control_types.h
  mas_listener.h
  mas_listeners.c
  mas_listeners.h
  mas_listener_wait_client.h

  mas_client.c
*/

/*
threads created:
  mas_server_main.c ("main")
  mas_listener_wait_client.c (mas_transaction_th)
  mas_master.c (mas_listener_th)
*/



/* static void                                   */
/* mas_server_cleanup( void *arg )               */
/* {                                             */
/*   tMSG( "%s in", __func__ );                  */
/*   thMSG( "%s MAIN TH. CLEANUP", __func__ );   */
/*   tMSG( "%s out", __func__ );                 */
/* }                                             */




int
mas_master_main( const mas_options_t * popts )
{
  EVAL_PREPARE;
  int r = 0;

  HMSG( "MASTER DO" );
  IEVAL( r, OPT_QFLAG( popts, make_master_thread ) ? mas_master_threaded( popts ) : mas_master_core( popts ) );

  return r;
}

__attribute__ ( ( constructor( 1001 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_constructor( IL, 0 );
}

__attribute__ ( ( destructor ) )
     static void mas_destructor( void )
{
  /* fprintf( stderr, "******************** DESTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_constructor( IL, 0 );
}
