#include <mastar/wrap/mas_std_def.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/types/mas_control_types.h>
/* #include <mastar/types/mas_opts_types.h> */
extern mas_control_t ctrl;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifndef MAS_SERVER_NOLOG
#  include <mastar/log/mas_logger.h>
#  include <mastar/log/mas_log.h>
#endif

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
mas_threads_init( MAS_PASS_OPTS_DECLARE1 )
{
  int r = 0;

  MAS_LOG( "(%d) init threads", r );
  mas_in_thread( MAS_THREAD_MAIN, NULL, NULL );

  pthread_rwlock_init( &ctrl.thglob.lcontrols_list_rwlock, NULL );
  pthread_rwlock_init( &ctrl.thglob.modules_list_rwlock, NULL );

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
  pthread_cond_init( &ctrl.thglob.logger_wait_cond, NULL );
  pthread_mutex_init( &ctrl.thglob.logger_wait_mutex, NULL );


  pthread_attr_init( &ctrl.thglob.master_attr );
  pthread_attr_init( &ctrl.thglob.ticker_attr );
  pthread_attr_init( &ctrl.thglob.watcher_attr );
  ctrl.thglob.master_set = CPU_ALLOC( 4 );
  {
    size_t size;

    size = CPU_ALLOC_SIZE( 4 );
    CPU_ZERO_S( size, ctrl.thglob.master_set );
    CPU_SET_S( 0, size, ctrl.thglob.master_set );
    pthread_attr_setaffinity_np( &ctrl.thglob.logger_attr, size, ctrl.thglob.master_set );
    pthread_attr_setaffinity_np( &ctrl.thglob.master_attr, size, ctrl.thglob.master_set );
    pthread_attr_setaffinity_np( &ctrl.thglob.ticker_attr, size, ctrl.thglob.master_set );
    pthread_attr_setaffinity_np( &ctrl.thglob.watcher_attr, size, ctrl.thglob.master_set );
  }

  pthread_attr_init( &ctrl.thglob.listener_attr );
  ctrl.thglob.listener_set = CPU_ALLOC( 4 );
  {
    size_t size;

    size = CPU_ALLOC_SIZE( 4 );
    CPU_ZERO_S( size, ctrl.thglob.listener_set );
    CPU_SET_S( 1, size, ctrl.thglob.listener_set );
    pthread_attr_setaffinity_np( &ctrl.thglob.listener_attr, size, ctrl.thglob.listener_set );
  }

  pthread_attr_init( &ctrl.thglob.transaction_attr );
  ctrl.thglob.transaction_set = CPU_ALLOC( 4 );
  {
    size_t size;

    size = CPU_ALLOC_SIZE( 4 );
    CPU_ZERO_S( size, ctrl.thglob.transaction_set );
    CPU_SET_S( 2, size, ctrl.thglob.transaction_set );
    CPU_SET_S( 3, size, ctrl.thglob.transaction_set );
    pthread_attr_setaffinity_np( &ctrl.thglob.transaction_attr, size, ctrl.thglob.transaction_set );
  }
  pthread_attr_setdetachstate( &ctrl.thglob.transaction_attr, PTHREAD_CREATE_DETACHED );


  {
    int r;

    size_t master_stacksize = 1024 * 40;
    size_t ticker_stacksize = 1024 * 50;
    size_t watcher_stacksize = 1024 * 50;
    size_t logger_stacksize = 1024 * 101;
    size_t listener_stacksize = 1024 * 50;
    size_t transaction_stacksize = 1024 * 32;

/* getrlimit(RLIMIT_STACK, &rlim); */
    r = pthread_attr_setstacksize( &ctrl.thglob.master_attr, master_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting master stack %lu (min:%lu)?", ( unsigned long ) master_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
    r = pthread_attr_setstacksize( &ctrl.thglob.ticker_attr, ticker_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting ticker stack %lu (min:%lu)?", ( unsigned long ) ticker_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
    r = pthread_attr_setstacksize( &ctrl.thglob.watcher_attr, watcher_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting watcher stack %lu (min:%lu)?", ( unsigned long ) watcher_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
    r = pthread_attr_setstacksize( &ctrl.thglob.logger_attr, logger_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting logger stack %lu (min:%lu)?", ( unsigned long ) logger_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
    r = pthread_attr_setstacksize( &ctrl.thglob.listener_attr, listener_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting listener stack %lu (min:%lu)?", ( unsigned long ) listener_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
    r = pthread_attr_setstacksize( &ctrl.thglob.transaction_attr, transaction_stacksize );
    if ( r )
    {
      P_ERR;
      EMSG( "setting transaction stack %lu (min:%lu)?", ( unsigned long ) transaction_stacksize, ( unsigned long ) PTHREAD_STACK_MIN );
    }
  }
  ctrl.threads.n.main.thread = pthread_self(  );
  ctrl.threads.n.main.tid = mas_gettid(  );
  MAS_LOG( "(%d) init threads done", r );
  return r;
}

void
mas_threads_destroy( MAS_PASS_OPTS_DECLARE1 )
{
  pthread_attr_destroy( &ctrl.thglob.custom_attr );

  pthread_attr_destroy( &ctrl.thglob.logger_attr );
  pthread_cond_destroy( &ctrl.thglob.logger_wait_cond );
  pthread_mutex_destroy( &ctrl.thglob.logger_wait_mutex );

  pthread_attr_destroy( &ctrl.thglob.master_attr );
  pthread_attr_destroy( &ctrl.thglob.ticker_attr );
  pthread_attr_destroy( &ctrl.thglob.watcher_attr );
  CPU_FREE( ctrl.thglob.master_set );
  ctrl.thglob.master_set = NULL;

  pthread_attr_destroy( &ctrl.thglob.listener_attr );
  CPU_FREE( ctrl.thglob.listener_set );
  ctrl.thglob.listener_set = NULL;

  pthread_attr_destroy( &ctrl.thglob.transaction_attr );
  CPU_FREE( ctrl.thglob.transaction_set );
  ctrl.thglob.transaction_set = NULL;

  pthread_rwlock_destroy( &ctrl.thglob.modules_list_rwlock );
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
