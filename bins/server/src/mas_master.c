#include <mastar/wrap/mas_std_def.h>

#include <unistd.h>
#include <fcntl.h>

#include <pthread.h>

#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include "mas_ticker.h"
#include "mas_watcher.h"

#include <mastar/log/mas_log.h>
#include <mastar/log/mas_logger.h>

#include <mastar/thtools/mas_thread_tools.h>

#include <mastar/init/mas_init.h>
#include <mastar/listener/mas_listeners.h>
#include <mastar/listener/mas_listener_control_list.h>

#include "mas_init_threads.h"
#include "mas_init_server.h"

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
/*   tMSG( "%s in", __func__ );                */
/*   thMSG( "%s MAIN TH. CLEANUP", __func__ ); */
/*   tMSG( "%s out", __func__ );               */
/* }                                             */



static int
mas_master( void )
{
  int r = 0;

  HMSG( "MASTER START %c%c%c", !opts.nologger ? 'L' : 'l', !opts.noticker ? 'T' : 't', !opts.nowatcher ? 'W' : 'w' );
  /* ??????? */
  /* r=0; */
  MAS_LOG( "to start spec. threads" );
  if ( !opts.nologger )
  {
    HMSG( "LOGGER TO START" );
    mas_logger_start(  );
  }
  if ( !opts.noticker )
  {
    HMSG( "TICKER TO START" );
    mas_ticker_start(  );
  }
  else
  {
    MAS_LOG( "running w/o ticker" );
  }
  if ( !opts.nowatcher )
  {
    HMSG( "WATCHER TO START" );
    mas_watcher_start(  );
  }
  else
  {
    MAS_LOG( "running w/o watcher" );
  }
  if ( opts.nomaster )
  {
    sleep( opts.nomaster );
  }
  else if ( opts.hosts_num > 0 && opts.hosts )
  {
    /* for ( int ih = 0; ih < opts.hosts_num; ih++ ) */
    /* {                                             */
    /*   thMSG( "%d. host %s", ih, opts.hosts[ih] ); */
    /* }                                             */
    while ( r >= 0 && ctrl.keep_listening && !ctrl.fatal )
    {
      MAS_LOG( "master loop for %d hosts", opts.hosts_num );
      tMSG( "master loop for %d hosts", opts.hosts_num );
      HMSG( "MASTER LOOP %dh", opts.hosts_num );
      /* mas_listeners.c */
      r = mas_listeners_start(  );

      HMSG( "WAITING..." );
      r = mas_listeners_wait(  );

      tMSG( "(%d) master loop for %d hosts", r, opts.hosts_num );

      {
        /* ???????? All listeners closed, what shall I do ?
         * 1. exit ( what is to be done with 'ctrl.keep_listening = 0' )
         * 2. re-run default listener(s)
         * 3. re-run defined ( opts.hosts ) listeners ( what is to be done without 'ctrl.keep_listening = 0' )
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
  if ( opts.exitsleep )
    sleep( opts.exitsleep );
  MAS_LOG( "to stop spec. threads" );
  FMSG( "TO STOP LOGGER" );
  mas_logger_stop(  );
  FMSG( "TO STOP TICKER" );
  mas_ticker_stop(  );
  MAS_LOG( "stopped spec. threads" );

  /* mas_watcher_stop(  ); */
  ctrl.watcher_stop = 1;

  MAS_LOG( "exiting master server" );
#ifdef MAS_TRACEMEM
  {
    extern unsigned long memory_balance;

    /* mMSG( "exiting master server, memory_balance:%lu", memory_balance ); */
    MAS_LOG( "master end, m/b:%lu", memory_balance );
  }
#endif
  HMSG( "MASTER_TH TO END : %d", r );
  return r;
}

static void *
mas_master_th( void *arg )
{
  int r = -1;

  HMSG( "MASTER_TH START" );
  ctrl.master_tid = mas_gettid(  );
  ctrl.master_pid = getpid(  );
  ctrl.server_pid = getpid(  );
  /* mas_malloc(1234); */
  MAS_LOG( "master starting @ %8.4f", ctrl.start_time );
  mas_in_thread( MAS_THREAD_MASTER, NULL, NULL );

  if ( ctrl.main_exit && ctrl.main_thread )
  {
    mas_pthread_detach( ctrl.main_thread );
    mas_xpthread_join( ctrl.main_thread );
    ctrl.main_thread = 0;
  }
  r = mas_master(  );
#ifdef MAS_TRACEMEM
  extern unsigned long memory_balance;

  /* mMSG( "mas_master_th end, memory_balance:%lu - Ticker:%lx;Logger:%lx;", memory_balance, ctrl.ticker_thread, ctrl.logger_thread ); */
  MAS_LOG( "mas_master_th end, m/b:%lu", memory_balance );
#endif
  HMSG( "MASTER_TH TO END" );
  mas_pthread_exit( NULL );
  return NULL;
}

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;
  if ( ctrl.stderrfile )
    fprintf( ctrl.stderrfile, "******************** CONSTRUCTOR %s\n", __FILE__ );
}

__attribute__ ( ( destructor ) )
     static void master_destructor( void )
{
  if ( ctrl.stderrfile )
    fprintf( ctrl.stderrfile, "******************** DESTRUCTOR %s\n", __FILE__ );
}

static int
mas_master_optional_thread( void )
{
  int r = 0;

  /* r = mas_xpthread_create( &master_thread, mas_master_th, MAS_THREAD_MASTER, ( void * ) NULL ); */
  if ( opts.make_master_thread )
  {
    r = pthread_create( &ctrl.master_thread, &ctrl.thglob.master_attr, mas_master_th, ( void * ) NULL );
    if ( !ctrl.main_exit && ctrl.master_thread )
    {
      mas_xpthread_join( ctrl.master_thread );
      ctrl.master_thread = ( pthread_t ) 0;
    }
  }
  else
  {
    r = mas_master(  );
  }
  return r;
}

int
mas_master_bunch( int argc, char *argv[], char *env[] )
{
  int r = -1;

  HMSG( "BUNCH START" );
  MAS_LOG( "bunch start" );
#ifdef MAS_INIT_SEPARATE
  r = mas_init_server( argc, argv, env );
#else
  MAS_LOG( "(%d) bunch: to init +", r );
  r = mas_init_plus( argc, argv, env, mas_init_pids, mas_init_daemon, mas_threads_init, mas_init_load_protos, mas_lcontrols_list_create,
                     NULL );
  MAS_LOG( "(%d) bunch: init + done", r );
#endif
  if ( ctrl.is_parent )
  {
    HMSG( "PARENT to exit" );
  }
  else if ( r >= 0 )
  {
    mas_master_optional_thread(  );
#ifdef MAS_TRACEMEM
    {
      extern unsigned long memory_balance;

      mMSG( "bunch end, memory_balance:%lu - Ticker:%lx;Logger:%lx;", memory_balance, ctrl.ticker_thread, ctrl.logger_thread );
      MAS_LOG( "bunch end, m/b:%lu", memory_balance );
    }
#endif
    HMSG( "BUNCH TO END" );
    mas_pthread_exit( &r );
  }
  MAS_LOG( "bunch end : %d", r );
  HMSG( "BUNCH %s END", ctrl.is_parent ? "(parent)" : "" );
  return r;
}
