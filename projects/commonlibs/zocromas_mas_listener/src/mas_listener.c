#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

/* ميخائيل */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* pthread_cleanup_push and pthread_cleanup_pop are macros and must always */
/* be used in matching pairs at the same nesting level of braces.          */
#include <pthread.h>
#include <sys/prctl.h>

#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>

#include <mastar/channel/mas_channel.h>
#include <mastar/channel/mas_channel_object.h>

#include <mastar/log/mas_log.h>

#include <mastar/thtools/mas_thread_tools.h>

#include <mastar/transaction/mas_transaction.h>

#include "mas_listener_wait_client.h"
#include "mas_lcontrol_object.h"
#include "mas_listener_control.h"

#include "mas_listener.h"


/*
this:
  mas_listener.c
related:
  mas_listener_wait_client.c
more:
  mas_lcontrol_object.c
  mas_lcontrol_object.h
  mas_listener_control.c
  mas_listener_control.h
  mas_listener_control_list.c
  mas_listener_control_list.h
  mas_listener_control_types.h
  mas_listener.h
  mas_listeners.c
  mas_listeners.h
  mas_listener_wait_client.h
  mas_server_main.c

  mas_transaction.c
  mas_cmdmod_listener.c

*/

extern mas_control_t ctrl;
extern mas_options_t opts;




/* (w/o stop) join */
int
mas_listener_wait( mas_lcontrol_t * plcontrol )
{
  int r = 0;
  pthread_t pth;

  /* thMSG( "waiting at M0:%u for L%lu:%u", ctrl.status, plcontrol->h.serial, plcontrol->h.status ); */
  if ( plcontrol && plcontrol->h.thread )
  {
    MAS_LOG( "to wait for listener [%lx] to stop", plcontrol->h.thread );
  }
  if ( plcontrol && ( pth = plcontrol->h.thread ) && 0 == mas_xpthread_join( pth ) )
  {
    r = -1;
    MAS_LOG( "stopped listener [%lx]", pth );
    plcontrol->h.thread = ( pthread_t ) 0;
    ctrl.status = MAS_STATUS_CLOSE;
    /* tMSG( "joined th %lx to %lx", ( unsigned long ) mas_pthread_self(  ), ( unsigned long ) plcontrol->h.thread ); */
    /* thMSG( "joined M0:%u & L%lu:%u", ctrl.status, plcontrol->h.serial, plcontrol->h.status ); */
    r = 0;
    WMSG( "LISTENER %lu STOPPED", plcontrol->h.serial );
  }
  return r;
}

/* static int                                                                                          */
/* mas_listener_join_transactions__( mas_lcontrol_t * plcontrol, int removeit )                        */
/* {                                                                                                   */
/*   (* mas_lcontrol_t *plcontrol = NULL; *)                                                           */
/*                                                                                                     */
/*   MAS_LOG( "to join tr.th" );                                                                       */
/*   (* plcontrol = mas_find_listener_control( mas_pthread_self(  ), NULL ); *)                        */
/*   (* FIXME  jwait + lock => deadlock ?!                                                             */
/*    *           TODO need cancel transactions before this                                            */
/*    *         or while + sleep ... ( and not jwait )                                                 */
/*    * *)                                                                                             */
/* (* mas_lcontrol_cleaning_transactions__ returns not-joined-count *)                                 */
/*   while ( mas_lcontrol_cleaning_transactions__( plcontrol, removeit, 700000000 (* jwait *)  ) > 0 ) */
/*   {                                                                                                 */
/*     (* thMSG( "WAIT MORE" ); *)                                                                     */
/*   }                                                                                                 */
/*   plcontrol->h.status = MAS_STATUS_STOP;                                                            */
/*   return 0;                                                                                         */
/* }                                                                                                   */

static size_t listener_stacksize = 0;
static void *listener_stackaddr = NULL;

/*
 * host : "mastar.lan:5003"
 * */
/* naming : setup + pthread_create = start */
int
mas_listener_start( char *host_port, unsigned port )
{
  int r = 0;

/* ~64M per listener -- why?? */
  if ( host_port )
  {
    mas_lcontrol_t *plcontrol;

    plcontrol = mas_lcontrol_make( host_port, port );
    {
      ( void ) pthread_attr_getstack( &ctrl.thglob.listener_attr, &listener_stackaddr, &listener_stacksize );
      lMSG( "cr. listener th. stack:%lu @ %p", ( unsigned long ) listener_stacksize, listener_stackaddr );
    }
    if ( opts.listener_single )
    {
      /* while ( ctrl.keep_listening ) */
      mas_listener( plcontrol );
    }
    else
    {
      MAS_LOG( "cr'ing ls. th; plc=%p #%lu", ( void * ) plcontrol, plcontrol->h.serial );
      /* r = mas_xpthread_create( &( plcontrol->h.thread ), mas_listener_th, MAS_THREAD_LISTENER, ( void * ) plcontrol ); */
      EEVAL( r, pthread_create( &plcontrol->h.thread, &ctrl.thglob.listener_attr, mas_listener_th, ( void * ) plcontrol ) );
      if ( plcontrol->h.thread )
      {
        MAS_LOG( "cr'ed ls. th; plc=%p [%lx] #%lu", ( void * ) plcontrol, plcontrol->h.thread, plcontrol->h.serial );
      }
    }
    if ( r == 0 )
    {
      lMSG( "<C l/th L%lu:%u for %s:%u", plcontrol->h.serial, plcontrol->h.status, plcontrol->host, plcontrol->port );
      ctrl.status = MAS_STATUS_OPEN;
    }
  }
  else
  {
    EMSG( "no host defined" );
    IEVAL( r, -1 );
  }
  return r;
}

/* naming : cancel + join = stop */
int
mas_listener_cancel( mas_lcontrol_t * plcontrol )
{
#ifdef MAS_TR_PERSIST
  mas_rcontrol_t *prcontrol;

  pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );
  MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )
  {
    int rcond;

    prcontrol->persistent_transaction = 0;
    rcond = pthread_cond_signal( &prcontrol->waitchan_cond );
    /* thMSG( "COND_SIGNAL :%d", rcond ); */
  }
  pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
#endif

  /* thMSG( "CANCEL th %lx", ( unsigned long ) plcontrol->h.thread ); */
  WMSG( "CANCEL L%lu:%u (plcontrol:%p) th:%lx", plcontrol->h.serial, plcontrol->h.status, ( void * ) plcontrol, plcontrol->h.thread );
  MAS_LOG( "cancelling L%lu:%u", plcontrol->h.serial, plcontrol->h.status );
  if ( plcontrol->h.thread )
    mas_pthread_cancel( plcontrol->h.thread );
  return 0;
}

static void
mas_listener_cleanup( void *arg )
{
  mas_lcontrol_t *plcontrol = NULL;

  if ( ( plcontrol = ( mas_lcontrol_t * ) arg ) )
  {
    MAS_LOG( "listener cleanup plc:%p", ( void * ) plcontrol );
    plcontrol->h.status = MAS_STATUS_END;
    MAS_LOG( "listener cleanup : to delete channel" );
    mas_channel_delete( plcontrol->h.pchannel, 0, 1 );
    plcontrol->h.pchannel = NULL;

    MAS_LOG( "listener cleanup : to end transactons" );
    /* TODO : move to another pthread ??? - NO */
    /* mas_listener_join_transactions__( plcontrol, 1 (* removeit *)  ); */
    MAS_LOG( "listener cleanup : transactons closed" );
  }
  else
  {
    EMSG( "listener cleanup - NO plcontrol" );
    MAS_LOG( "listener cleanup - NO plcontrol" );
  }
  /* thMSG( "cleanup for l/th %lx", mas_pthread_self(  ) ); */
  /* thMSG( "C DELETE %d %p", __LINE__, ( void * ) plcontrol ); */
  /* NOT HERE : mas_lcontrol_remove_delete( plcontrol ); */
  MAS_LOG( "listener cleanup; to delete plc:%p in list:%d", ( void * ) plcontrol, plcontrol->in_list );
  /* mas_in_thread_end(  ); */
  mas_lcontrol_remove_delete( plcontrol );
}

int
mas_listener( mas_lcontrol_t * plcontrol )
{
  int r = -1;

  plcontrol->h.status = MAS_STATUS_INIT;

  MAS_LOG( "l/th loop" );
  while ( !ctrl.fatal && 0 == ( r = mas_listener_wait_client( plcontrol ) ) )
  {
    MAS_LOG( "listener next client ..." );
  }
  plcontrol->h.status = MAS_STATUS_CLOSE;
  /* thMSG( "stopped listening r:%d", r ); */
  MAS_LOG( "exiting listening (stopped listening) r:%d", r );
  /* mas_listener_join_transactions( &channel.transaction_threads ); */
  return r;
}

/* naming : pthread_create argument = th */
void *
mas_listener_th( void *tlcontrol )
{
  int rn = 0;
  mas_lcontrol_t *plcontrol = NULL;

  plcontrol = ( mas_lcontrol_t * ) tlcontrol;
  IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocListen" ) );

  /* {                                                                       */
  /*   int rs;                                                               */
  /*   struct sched_param sched;                                             */
  /*                                                                         */
  /*   sched.sched_priority = 1;                                             */
  /*   rs = pthread_setschedparam( mas_pthread_self(  ), SCHED_RR, &sched ); */
  /*   MAS_LOG( "sched %d", rs );                                            */
  /* }                                                                       */
  if ( opts.nolisten )
  {
    MAS_LOG( "listener th. -> sleep started %s:%u", plcontrol->host, plcontrol->port );
    sleep( opts.nolisten );
  }
  else if ( plcontrol )
  {
    MAS_LOG( "listener th. started %s:%u", plcontrol->host, plcontrol->port );
    mas_in_thread( MAS_THREAD_LISTENER, plcontrol, NULL );
    /* thMSG( "mas_listener_th %s:%u", plcontrol->host, plcontrol->port ); */
    plcontrol->h.tid = mas_gettid(  );
    if ( !plcontrol->host )
    {
      EMSG( "host not set" );
      MAS_LOG( "listener: host not set" );
    }
    plcontrol->h.status = MAS_STATUS_START;
#ifdef MAS_TR_PERSIST
    {
      for ( int it = 0; it < 10; it++ )
      {
        /* thMSG( "Pre-tr. plcontrol->channel; opened : %d", plcontrol->h.pchannel ? plcontrol->h.pchannel->opened : -1 ); */
        mas_transaction_start( plcontrol, 1 /* persistent tr. */  );
      }
    }
#endif
    {
      /* int r = 0; */

      /* thMSG( "setting cleanup for L %s:%u", plcontrol->host, plcontrol->port ); */
      MAS_LOG( "setting cleanup for L %s:%u", plcontrol->host, plcontrol->port );
      pthread_cleanup_push( mas_listener_cleanup, plcontrol );
      ( void ) /*r = */ mas_listener( plcontrol );
      pthread_cleanup_pop( 1 );
    }
  }

  MAS_LOG( "listener th. ending" );

  /* mas_lcontrols_delete( plcontrol );  ---> done at mas_listener_cleanup ?!?! */

  EMSG( "LISTENER STOP" );
  IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocListenXit" ) );
  mas_pthread_exit( NULL );
  return NULL;
}
