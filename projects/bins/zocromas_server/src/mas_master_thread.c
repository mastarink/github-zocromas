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

#include <mastar/thtools/mas_thread_tools.h>

#include "mas_master_core.h"
#include "mas_master_thread.h"

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


static void *
mas_master_th( void *topts )
{
  CTRL_PREPARE;
  int rn = 0;
  const mas_options_t *popts;

  popts = ( mas_options_t * ) topts;
  HMSG( "MASTER_TH START" );
  ctrl.threads.n.master.tid = mas_gettid(  );
  ctrl.threads.n.master.pid = getpid(  );
  ctrl.threads.n.master.thread = mas_pthread_self(  );
  ctrl.pserver_thread = &ctrl.threads.n.master;

  ctrl.pserver_thread = &ctrl.threads.n.master;
  {
    EVAL_PREPARE;
    IEVAL( rn, mas_thself_set_name( ctrl.is_parent ? popts->thname.parent_masterth : popts->thname.daemon_masterth, "zocMasterTh" ) );
  }
  MAS_LOG( "master starting @ %8.4f", ctrl.start_time );
  mas_in_thread( MAS_THREAD_MASTER, NULL, NULL );

  if ( ctrl.main_exit && ctrl.threads.n.main.thread )
  {
    /* FIXME ????? */
    mas_pthread_detach( ctrl.threads.n.main.thread );
    mas_xpthread_join( ctrl.threads.n.main.thread );
    ctrl.threads.n.main.thread = 0;
  }
  {
    ( void ) /* r= */ mas_master_core( popts );
  }
  HMSG( "MASTER_TH TO END" );
  {
    EVAL_PREPARE;
    IEVAL( rn, mas_thself_set_name( ctrl.is_parent ? popts->thname.parent_masterthx : popts->thname.daemon_masterthx, "zocMasterThX" ) );
  }
  mas_pthread_exit( NULL );
  return NULL;
}

int
mas_master_threaded( const mas_options_t * popts )
{
  EVAL_PREPARE;
  int r = 0;

  IEVAL( r, pthread_create( &ctrl.threads.n.master.thread, &ctrl.thglob.master_attr, mas_master_th, ( void * ) popts ) );
  if ( !ctrl.main_exit && ctrl.threads.n.master.thread )
  {
    mas_xpthread_join( ctrl.threads.n.master.thread );
    ctrl.threads.n.master.thread = ( pthread_t ) 0;
  }
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
