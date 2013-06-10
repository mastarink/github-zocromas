#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <sys/time.h>

#include <pthread.h>
#include <sys/prctl.h>

#include <mastar/channel/mas_channel_open.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_control_types.h>
/* #include <mastar/types/mas_opts_types.h> */
extern mas_control_t ctrl;

#include <mastar/log/mas_log.h>

#include <mastar/transaction/mas_transaction_control.h>
#include <mastar/transaction/mas_transaction.h>

#include "mas_listener_control.h"
#include "mas_listener_wait_client.h"


/*
this:
  mas_listener_wait_client.c
related:
  mas_server_master.c

  mas_transaction.c
  mas_transaction_http.c
  mas_transaction_xcromas.c

  mas_lib_thread.c

threads created:
  mas_server_main.c ("main")
  mas_listener_wait_client.c (mas_transaction_th)
  mas_server_master.c (mas_server_listener_th)
*/


/*
 * create transaction thread;
 * server -> transaction
*/
mas_rcontrol_t *
mas_listener_find_free_transaction( mas_lcontrol_t * plcontrol )
{
  mas_rcontrol_t *prcontrol = NULL;
  int cnt = 0;

  pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );

  MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )
  {
    cnt++;
    if ( mas_channel_opened( prcontrol->h.pchannel ) )
      continue;
    if ( prcontrol->waitchan_waiting )
      break;
  }
  pthread_rwlock_unlock( &plcontrol->transaction_rwlock );

  return prcontrol;
}

mas_rcontrol_t *
mas_listener_find_free_transaction__( mas_lcontrol_t * plcontrol )
{
  mas_rcontrol_t *prcontrol = NULL;

  pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );

  MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )
  {
    if ( !( prcontrol->h.pchannel && prcontrol->h.pchannel->opened ) && prcontrol->waitchan_waiting )
      break;
  }
  pthread_rwlock_unlock( &plcontrol->transaction_rwlock );

  return prcontrol;
}

int
mas_listener_wait_client( MAS_PASS_OPTS_DECLARE mas_lcontrol_t * plcontrol )
{
  int r = 0, ro = 0, rn = 0;
  mas_rcontrol_t *prcontrol = NULL;

  plcontrol->h.status = MAS_STATUS_WAIT;
  OMSG( "WAITING CLIENT" );
  MAS_LOG( "waiting client" );
  if ( plcontrol )
  {
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocListenW" ) );

    if ( plcontrol->h.pchannel->opened )
    {
      MAS_LOG( "why OPENED before open?" );
    }
    /* wMSG( "to open channel opened : %d", plcontrol->h.pchannel->opened ); */
    {
      plcontrol->h.pchannel->cloned = 0;
      IEVAL( r, mas_channel_open( plcontrol->h.pchannel ) );
      ro = r;
/* ?????? fcntl(fd, F_SETFD, FD_CLOEXEC) */
      MAS_LOG( "(%d) opened channel ========", r );
    }
    IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocListenIn" ) );
    OMSG( "INCOMING CONNECTION" );
    {
      struct timeval td;

      gettimeofday( &td, NULL );
      plcontrol->h.activity_time = td;
    }


    IEVAL( r, ctrl.stop_listeners ? -1 : 0 );



    /* wMSG( "(%d) opened? channel; opened : %d", r, plcontrol->h.pchannel->opened ); */
    if ( r < 0 )
    {
      MAS_LOG( "(%d) NOT opened? channel; opened : %d", r, plcontrol->h.pchannel->opened );
    }
    else if ( 0 && ( prcontrol = mas_listener_find_free_transaction( plcontrol ) ) )
    {
      /* int rcond; */

      MAS_LOG( "(%d)found free tr. (s%lu); at l opened:%d", r, plcontrol->h.serial,
               plcontrol->h.pchannel ? plcontrol->h.pchannel->opened : 0 );
      IEVAL( r, mas_rcontrol_set_channel( prcontrol, plcontrol->h.pchannel ) );
      MAS_LOG( "cond signal to R%lu", prcontrol->h.serial );
      ( void ) /*rcond = */ pthread_cond_signal( &prcontrol->waitchan_cond );
    }
    else
    {
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) "zocListenT" ) );

      /* plcontrol->h.status = MAS_STATUS_OPEN; */
      plcontrol->h.status = MAS_STATUS_WORK;
#ifdef MAS_TR_PERSIST
      r = mas_transaction_start( MAS_PASS_OPTS_PASS plcontrol, 0 /* persistent tr. */  );
#else
      r = mas_transaction_start( MAS_PASS_OPTS_PASS plcontrol );
#endif
      MAS_LOG( "transaction started r:%d", r );
    }
    MAS_LOG( "opened : %d", plcontrol->h.pchannel->opened );
//  tMSG( "to close2" );
//  {
//    int rcl;

//    /* preserve earlier error */
//    MAS_LOG( "cloned : %d", plcontrol->h.pchannel->cloned );
//    rcl = mas_channel_close2( plcontrol->h.pchannel );
//    MAS_LOG( "(%d) close2", rcl );
//    if ( r >= 0 )
//    {
//      r = rcl;
//    }
//  }

    /* mas_lcontrols_cleaning_transactions( ctrl.forget_transactions, 0 ); */
/* mas_lcontrol_cleaning_transactions__ returns not-joined-count - poh here */
    MAS_LOG( "by the way: cleaning transactions" );
    /* mas_lcontrol_cleaning_transactions__( plcontrol, ctrl.forget_transactions, 0 (* don't wait *)  ); */
  }
  else
  {
#ifdef EMSG
    EMSG( "no plcontrol" );
#endif
    /* ctrl.keep_listening = 0; */
  }

  if ( !ctrl.keep_listening )
  {
    /* wMSG( "not ctrl.keep_listening" ); */
    r = 7;
    MAS_LOG( "not ctrl.keep_listening r:%d", r );
  }
  else
  {
    /* tMSG( "ctrl.keep_listening" ); */
  }
  /* wMSG( "(%d) stop waiting", r ); */
  return ro < 0 ? ( r > 0 ? r : 0 ) : r;
}
