#include "mas_server_def.h"
#include "mas_basic_def.h"

#include <fcntl.h>

#include <pthread.h>

#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include "mas_ticker.h"
#include "mas_watcher.h"

#include <mastar/log/mas_logger.h>

#include <mastar/thtools/mas_thread_tools.h>

#include "listener/inc/mas_listeners.h"
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



void *
mas_master_th( void *arg )
{
  int r = -1;

  ctrl.master_tid = mas_gettid(  );
  /* mas_malloc(1234); */
  MAS_LOG( "master starting @ %8.4f", ctrl.start_time );
  mas_in_thread( MAS_THREAD_MASTER, NULL, NULL );

  if ( ctrl.main_exit && ctrl.main_thread )
  {
    mas_pthread_detach( ctrl.main_thread );
    mas_xpthread_join( ctrl.main_thread );
    ctrl.main_thread = 0;
  }
  if ( ctrl.daemon )
  {
    int foutd = -1;
    int ferrd = -1;

    foutd = mas_open( "daemon_stdout.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0777 );
    ferrd = mas_open( "daemon_stderr.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0777 );
    ctrl.saved_stderr = dup( STDERR_FILENO );
    ctrl.saved_stderr_file = fdopen( ctrl.saved_stderr, "w" );
    ctrl.saved_stdout = dup( STDOUT_FILENO );
    dup2( foutd, STDOUT_FILENO );
    dup2( ferrd, STDERR_FILENO );
    mas_close( foutd );
    mas_close( ferrd );
  }
  r = mas_master(  );
  if ( 0 && ctrl.daemon )
  {
    mas_close( STDOUT_FILENO );
    mas_close( STDERR_FILENO );
  }
  /* mas_destroy_server(  ); */
#ifdef MAS_TRACEMEM
  extern unsigned long memory_balance;

  mMSG( "bunch end, memory_balance:%lu - Ticker:%lx;Logger:%lx;", memory_balance, ctrl.ticker_thread, ctrl.logger_thread );
  MAS_LOG( "bunch end, m/b:%lu", memory_balance );
#endif
  MAS_LOG( "to stop spec. threads" );
  FMSG( "TO STOP LOGGER" );
  mas_logger_stop(  );
  FMSG( "TO STOP TICKER" );
  mas_ticker_stop(  );
  MAS_LOG( "stopped spec. threads" );

  /* mas_watcher_stop(  ); */
  ctrl.watcher_stop = 1;
  mas_pthread_exit( NULL );
  return NULL;
}


/*
Creating a daemon
   1 Create a normal process (Parent process)
   2 Create a child process from within the above parent process
   3 The process hierarchy at this stage looks like :  TERMINAL -> PARENT PROCESS -> CHILD PROCESS
   4 Terminate the the parent process.
   5 The child process now becomes orphan and is taken over by the init process.
   6 Call setsid() function to run the process in new session and have a new group.
   7 After the above step we can say that now this process becomes a daemon process without having a controlling terminal.
   8 Change the working directory of the daemon process to root and close stdin, stdout and stderr file descriptors.
   9 Let the main logic of daemon process run.
*/

/*
 * logger + ticker = 179
 * logger = 107
 * ticker =
 * */
int
mas_master( void )
{
  int r = 0;

  /* ??????? */
  /* r=0; */
  MAS_LOG( "to start spec. threads" );
  if ( !opts.nologger )
    mas_logger_start(  );
  if ( !opts.noticker )
    mas_ticker_start(  );
  else
  {
    MAS_LOG( "running w/o ticker" );
  }
  if ( !opts.nowatcher )
    mas_watcher_start(  );
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
      thMSG( "master loop for %d hosts", opts.hosts_num );
  /* mas_listeners.c */
      r = mas_listeners_start(  );

      thMSG( "waiting..." );

      r = mas_listeners_wait(  );

      thMSG( "(%d) master loop for %d hosts", r, opts.hosts_num );

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

  MAS_LOG( "exiting master server" );
#ifdef MAS_TRACEMEM
  extern unsigned long memory_balance;

  mMSG( "exiting master server, memory_balance:%lu", memory_balance );
  MAS_LOG( "master end, m/b:%lu", memory_balance );
#endif
  return r;
}

int
mas_master_bunch( int argc, char *argv[], char *env[] )
{
  int r = 0;

  ctrl.main_tid = mas_gettid(  );

  MAS_LOG( "bunch start" );
  ctrl.status = MAS_STATUS_START;
  ctrl.start_time = mas_double_time(  );
  mas_init_server( mas_atexit, 1, argc, argv, env );

  /* r = mas_xpthread_create( &master_thread, mas_master_th, MAS_THREAD_MASTER, ( void * ) NULL ); */
  r = pthread_create( &ctrl.master_thread, &ctrl.thglob.master_attr, mas_master_th, ( void * ) NULL );
  if ( !ctrl.main_exit && ctrl.master_thread )
  {
    mas_xpthread_join( ctrl.master_thread );
    ctrl.master_thread = 0;
  }
  MAS_LOG( "bunch end : %d", r );
  return r;
}
