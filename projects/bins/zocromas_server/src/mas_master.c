#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <unistd.h>

#include <pthread.h>

#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_control_types.h>

#include <mastar/log/mas_log.h>
#include <mastar/log/mas_logger.h>

#include <mastar/thtools/mas_thread_tools.h>


#include <mastar/listener/mas_listeners.h>

#include "mas_ticker.h"
#include "mas_watcher.h"

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



static int
mas_master( const mas_options_t * popts )
{
  CTRL_PREPARE;
  int r = 0, rn = 0;

  HMSG( "MASTER START : pid:%u %c%c%c", getpid(  ), popts->log.run ? 'L' : 'l', !popts->noticker ? 'T' : 't',
        !popts->nowatcher ? 'W' : 'w' );
  /* if ( ctrl.is_parent )                                                  */
  /* {                                                                      */
  /*   IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParMaster" ) ); */
  /* }                                                                      */
  /* else                                                                   */
  /* {                                                                      */
  /*   IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaeMaster" ) ); */
  /* }                                                                      */
  /* ??????? */
  /* r=0; */
  MAS_LOG( "to start spec. threads" );
  if ( popts->log.run )
  {
    HMSG( "LOGGER TO START" );
    mas_logger_start(  );
  }
  else
  {
    HMSG( "NO LOGGER" );
  }
  if ( !popts->noticker )
  {
    HMSG( "TICKER TO START" );
    mas_ticker_start( popts );
  }
  else
  {
    MAS_LOG( "running w/o ticker" );
    HMSG( "NO TICKER" );
  }
  if ( !popts->nowatcher )
  {
    HMSG( "WATCHER TO START" );
    mas_watcher_start( popts );
  }
  else
  {
    MAS_LOG( "running w/o watcher" );
    HMSG( "NO WATCHER" );
  }
  if ( popts->nomaster )
  {
    sleep( popts->nomaster );
  }
  else if ( popts->hostsv.c > 0 && popts->hostsv.v )
  {
    /* for ( int ih = 0; ih < popts->  hostsv.c; ih++ ) */
    /* {                                             */
    /*   thMSG( "%d. host %s", ih, popts->  hostsv.v[ih] ); */
    /* }                                             */
    while ( r >= 0 && ctrl.keep_listening && !ctrl.fatal && !popts->quit )
    {
      int qstopped = 0;

      MAS_LOG( "master loop for %d hosts", popts->hostsv.c );
      tMSG( "master loop for %d hosts", popts->hostsv.c );
      HMSG( "MASTER LOOP %d host; parent:%d", popts->hostsv.c, ctrl.is_parent );
      /* mas_listeners.c */
      r = mas_listeners_start( popts );

      HMSG( "WAITING..." );
      qstopped = mas_listeners_wait(  );
      HMSG( "STOPPED lsn's (%d)", qstopped );

      tMSG( "(%d) master loop for %d hosts", r, popts->hostsv.c );

      {
        /* ???????? All listeners closed, what shall I do ?
         * 1. exit ( what is to be done with 'ctrl.keep_listening = 0' )
         * 2. re-run default listener(s)
         * 3. re-run defined ( popts->  hostsv.v ) listeners ( what is to be done without 'ctrl.keep_listening = 0' )
         * */
        if ( MAS_LIST_EMPTY( ctrl.lcontrols_list ) )
        {
          ctrl.keep_listening = 0;
          MAS_LOG( "listeners list is empty" );
        }
        else
        {
          MAS_LOG( "listeners list NOT is empty" );
        }
      }
    }
    thMSG( "just out of server main loop" );
    MAS_LOG( "just out of server main loop" );
  }
  else
  {
    EMSG( "hosts not defined" );
    MAS_LOG( "hosts not defined" );
  }
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
    ( void ) /* r= */ mas_master( popts );
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
mas_master_do( const mas_options_t * popts )
{
  EVAL_PREPARE;
  int r = 0;

  /* r = mas_xpthread_create( &master_thread, mas_master_th, MAS_THREAD_MASTER, ( void * ) NULL ); */
  if ( popts->make_master_thread )
  {
    /* r = pthread_create( &ctrl.threads.n.master.thread, &ctrl.thglob.master_attr, mas_master_th, ( void * ) NULL ); */
    IEVAL( r, pthread_create( &ctrl.threads.n.master.thread, &ctrl.thglob.master_attr, mas_master_th, ( void * ) popts ) );
    if ( !ctrl.main_exit && ctrl.threads.n.master.thread )
    {
      mas_xpthread_join( ctrl.threads.n.master.thread );
      ctrl.threads.n.master.thread = ( pthread_t ) 0;
    }
  }
  else
  {
    /* r = mas_master(  ); */
    IEVAL( r, mas_master( popts ) );
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
