#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <unistd.h>
#include <fcntl.h>

#include <pthread.h>
#include <sys/prctl.h>


#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/modules/mas_modules_ctrl_module.h>
/* #include <mastar/modules/mas_modules_load_module.h> */

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

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;


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
mas_master( mas_options_t * popts )
{
  int r = 0, rn = 0;

  HMSG( "MASTER START : pid:%u %c%c%c", getpid(  ), !popts->nologger ? 'L' : 'l', !popts->noticker ? 'T' : 't',
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
  if ( !popts->nologger )
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
    while ( r >= 0 && ctrl.keep_listening && !ctrl.fatal )
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
#ifdef MAS_TRACEMEM
  {
    extern unsigned long memory_balance;

    /* mMSG( "exiting master server, memory_balance:%lu", memory_balance ); */
    MAS_LOG( "master end, m/b:%lu", memory_balance );
  }
#endif
  HMSG( "MASTER TO END : %d", r );
  if ( ctrl.is_parent )
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParMasterX" ) );
  }
  else
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaeMasterX" ) );
  }
  return r;
}

static void *
mas_master_th( void *arg )
{
  int rn = 0;

  HMSG( "MASTER_TH START" );
  ctrl.threads.n.master.tid = mas_gettid(  );
  ctrl.threads.n.master.pid = getpid(  );
  ctrl.threads.n.master.thread = mas_pthread_self(  );
  ctrl.pserver_thread = &ctrl.threads.n.master;

  ctrl.pserver_thread = &ctrl.threads.n.master;
  if ( ctrl.is_parent )
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParMasterTh" ) );
  }
  else
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaeMasterTh" ) );
  }
  /* mas_malloc(1234); */
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
    extern mas_options_t gopts;

    ( void ) /* r= */ mas_master( &gopts );
  }
#ifdef MAS_TRACEMEM
  extern unsigned long memory_balance;

  /* mMSG( "mas_master_th end, memory_balance:%lu - Ticker:%lx;Logger:%lx;", memory_balance, ctrl.threads.n.ticker.thread, ctrl.threads.n.logger.thread ); */
  MAS_LOG( "mas_master_th end, m/b:%lu", memory_balance );
#endif
  HMSG( "MASTER_TH TO END" );
  if ( ctrl.is_parent )
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParMasterThX" ) );
  }
  else
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaeMasterThX" ) );
  }
  mas_pthread_exit( NULL );
  return NULL;
}

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  /* if ( stderr )                                                                      */
  /*   fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno ); */
  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;
}

__attribute__ ( ( destructor ) )
     static void master_destructor( void )
{
  /* if ( stderr )                                                                     */
  /*   fprintf( stderr, "******************** DESTRUCTOR %s e%d\n", __FILE__, errno ); */
}

static int
mas_master_optional_thread( mas_options_t * popts )
{
  int r = 0;

  /* r = mas_xpthread_create( &master_thread, mas_master_th, MAS_THREAD_MASTER, ( void * ) NULL ); */
  if ( popts->make_master_thread )
  {
    /* r = pthread_create( &ctrl.threads.n.master.thread, &ctrl.thglob.master_attr, mas_master_th, ( void * ) NULL ); */
    IEVAL( r, pthread_create( &ctrl.threads.n.master.thread, &ctrl.thglob.master_attr, mas_master_th, ( void * ) NULL ) );
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

int
mas_master_bunch( mas_options_t * popts, int argc, char *argv[], char *env[] )
{
  int r = 0, rn = 0;

  HMSG( "BUNCH START e:%d", errno );
  MAS_LOG( "bunch start e:%d", errno );

  if ( ctrl.is_parent )
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParBunchI" ) );
  }
  else
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaeBunchI" ) );
  }
  MAS_LOG( "(%d) bunch: to init +", r );
  /* r = mas_init_plus( argc, argv, env, mas_init_pids, mas_init_daemon, mas_threads_init, mas_init_load_protos, mas_lcontrols_list_create, */
  /*                    NULL );                                                                                                             */
  IEVAL( r,
         mas_init_plus( popts, argc, argv, env, mas_init_daemon, mas_init_pids, mas_threads_init, mas_init_load_protos, mas_lcontrols_init,
                        NULL ) );
  if ( ctrl.is_parent )
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParBunch" ) );
  }
  else
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaeBunch" ) );
  }
  MAS_LOG( "(%d) bunch: init + done", r );
  if ( ctrl.is_parent )
  {
    HMSG( "PARENT to exit" );
  }
  else                          /* if ( r >= 0 ) */
  {
    IEVAL( r, mas_master_optional_thread( popts ) );
#ifdef MAS_TRACEMEM
    {
      extern unsigned long memory_balance;

      mMSG( "bunch end, memory_balance:%lu - Ticker:%lx;Logger:%lx;", memory_balance, ctrl.threads.n.ticker.thread,
            ctrl.threads.n.logger.thread );
      MAS_LOG( "bunch end, m/b:%lu", memory_balance );
#  if 0
      if ( print_memlist( ctrl.stderrfile, FL ) < 0 )
        if ( print_memlist( ctrl.old_stderrfile, FL ) < 0 )
          if ( print_memlist( ctrl.msgfile, FL ) < 0 )
            print_memlist( stderr, FL );
#  endif
    }
#endif
    HMSG( "BUNCH TO END" );
    /* XXX XXX XXX [ if use mas_pthread_exit - forever wait 'foreign' threads ] XXX XXX XXX
       mas_pthread_exit( &r ); 
     */
  }
  if ( ctrl.is_parent )
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocParBunchX" ) );
  }
  else
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocDaeBunchX" ) );
  }
  WMSG( "TO DESTROY MODULES" );
  mas_modules_unregister(  );
  /* MAS_LOG( "bunch end : %d", r ); */
  for ( int ith = 0; ith < sizeof( ctrl.threads.a ) / sizeof( ctrl.threads.a[0] ); ith++ )
  {
    WMSG( "TO JOIN #%d of %u", ith, ( unsigned ) ( sizeof( ctrl.threads.a ) / sizeof( ctrl.threads.a[0] ) ) );
    if ( ctrl.threads.a[ith].thread )
      mas_xpthread_join( ctrl.threads.a[ith].thread );
    ctrl.threads.a[ith].thread = 0;
  }
  WMSG( "/ JOIN" );
  WMSG( "BUNCH END DATA master:[%lx] log:[%lx] t[%lx] w[%lx] %d", ctrl.threads.n.master.thread,
        ctrl.threads.n.logger.thread, ctrl.threads.n.ticker.thread, ctrl.threads.n.watcher.thread, ctrl.lcontrols_list ? 1 : 0 );
  HMSG( "BUNCH %s END", ctrl.is_parent ? "(parent)" : "" );
  return r;
}
