#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/thtools/mas_thread_tools.h>

#include "mas_init_threads.h"

/*
this:
  mas_init_threads.c
related:
  mas_init.c
  mas_init_server.c
  mas_init_client.c
more:
  mas_init_client.h
  mas_init.h
  mas_init_server.h
  mas_init_threads.c
  mas_init_threads.h

  mas_opts.c
*/


int
mas_threads_init( void )
{
  mas_in_thread( MAS_THREAD_MAIN, NULL, NULL );

  pthread_rwlock_init( &ctrl.thglob.lcontrols_list_rwlock, NULL );

  pthread_mutex_init( &ctrl.thglob.malloc_mutex, NULL );
  pthread_mutex_init( &ctrl.thglob.cleanup_transactions_mutex, NULL );
  pthread_mutex_init( &ctrl.thglob.msg_mutex, NULL );
  pthread_mutex_init( &ctrl.thglob.emsg_mutex, NULL );
  pthread_mutex_init( &ctrl.thglob.mfp_mutex, NULL );
  pthread_mutex_init( &ctrl.thglob.cnttr1_mutex, NULL );
  pthread_mutex_init( &ctrl.thglob.cnttr2_mutex, NULL );
//pthread_mutex_init( &ctrl.thglob.log_mutex, NULL );

  pthread_attr_init( &ctrl.thglob.custom_attr );
  pthread_attr_init( &ctrl.thglob.logger_attr );
  pthread_attr_init( &ctrl.thglob.master_attr );
  pthread_attr_init( &ctrl.thglob.ticker_attr );
  pthread_attr_init( &ctrl.thglob.watcher_attr );
  pthread_attr_init( &ctrl.thglob.listener_attr );
  pthread_attr_init( &ctrl.thglob.transaction_attr );

  {
    int r;

    size_t master_stacksize =  1024 * 40;
    size_t ticker_stacksize =  1024 * 50;
    size_t watcher_stacksize =  1024 * 50;
    size_t logger_stacksize =  1024 * 100;
    size_t listener_stacksize =  1024 * 50;
    size_t transaction_stacksize =  1024 * 32;

/* getrlimit(RLIMIT_STACK, &rlim); */
    r = pthread_attr_setstacksize( &ctrl.thglob.master_attr, master_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting master stack %lu (min:%lu)?", master_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
    r = pthread_attr_setstacksize( &ctrl.thglob.ticker_attr, ticker_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting ticker stack %lu (min:%lu)?", ticker_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
    r = pthread_attr_setstacksize( &ctrl.thglob.watcher_attr, watcher_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting watcher stack %lu (min:%lu)?", watcher_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
    r = pthread_attr_setstacksize( &ctrl.thglob.logger_attr, logger_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting logger stack %lu (min:%lu)?", logger_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
    r = pthread_attr_setstacksize( &ctrl.thglob.listener_attr, listener_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting listener stack %lu (min:%lu)?", listener_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
       r = pthread_attr_setstacksize( &ctrl.thglob.transaction_attr, transaction_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting transaction stack %lu (min:%lu)?", transaction_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
  }
  ctrl.main_thread = pthread_self(  );
  ctrl.main_tid = mas_gettid(  );
  return 0;
}

void
mas_threads_destroy( void )
{
  pthread_attr_destroy( &ctrl.thglob.custom_attr );
  pthread_attr_destroy( &ctrl.thglob.logger_attr );
  pthread_attr_destroy( &ctrl.thglob.master_attr );
  pthread_attr_destroy( &ctrl.thglob.ticker_attr );
  pthread_attr_destroy( &ctrl.thglob.watcher_attr );
  pthread_attr_destroy( &ctrl.thglob.listener_attr );
  pthread_attr_destroy( &ctrl.thglob.transaction_attr );

  pthread_rwlock_destroy( &ctrl.thglob.lcontrols_list_rwlock );

  pthread_mutex_destroy( &ctrl.thglob.mfp_mutex );
  pthread_mutex_destroy( &ctrl.thglob.msg_mutex );
  pthread_mutex_destroy( &ctrl.thglob.emsg_mutex );
  pthread_mutex_destroy( &ctrl.thglob.cleanup_transactions_mutex );
  pthread_mutex_destroy( &ctrl.thglob.malloc_mutex );
  pthread_mutex_destroy( &ctrl.thglob.cnttr1_mutex );
  pthread_mutex_destroy( &ctrl.thglob.cnttr2_mutex );
//pthread_mutex_destroy( &ctrl.thglob.log_mutex );
}
