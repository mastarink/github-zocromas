#include <sys/time.h>

#include <pthread.h>
#include <sys/prctl.h>

#include <mastar/types/mas_control_types.h>
/* #include <mastar/types/mas_opts_types.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/channel/mas_channel_open.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

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
mas_listener_client_came( mas_lcontrol_t * plcontrol )
{
  EVAL_PREPARE;
  mas_rcontrol_t *prcontrol = NULL;
  int r = 0, rn = 0;

  /* wMSG( "(%d) opened? channel; opened : %d", r, plcontrol->h.pchannel->opened ); */  
  if ( 0 && ( prcontrol = mas_listener_find_free_transaction( plcontrol ) ) )
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
    if ( plcontrol->popts->thname.listent )
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) plcontrol->popts->thname.listent /* "zocListenT" */  ) );

    /* plcontrol->h.status = MAS_STATUS_OPEN; */
    plcontrol->h.status = MAS_STATUS_START;
#ifdef MAS_TR_PERSIST
    r = mas_transaction_start( plcontrol, 0 /* persistent tr. */  );
#else
    r = mas_transaction_start( plcontrol );
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
  return r;
}

int
mas_listener_wait_client( mas_lcontrol_t * plcontrol )
{
  int r = 0, ro = 0, rn = 0;

  plcontrol->h.status = MAS_STATUS_SERV_LOOP;
  OMSG( "WAITING CLIENT" );
  MAS_LOG( "waiting client" );
  if ( plcontrol )
  {
    EVAL_PREPARE;
    if ( plcontrol->popts->thname.listenw )
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) plcontrol->popts->thname.listenw /* "zocListenW" */  ) );

    /* if ( plcontrol->h.pchannel->opened )    */
    /* {                                       */
    /*   MAS_LOG( "why OPENED before open?" ); */
    /* }                                       */
    /* wMSG( "to open channel opened : %d", plcontrol->h.pchannel->opened ); */
    {
      plcontrol->h.pchannel->cloned = 0;
      IEVAL( r, mas_channel_open( plcontrol->h.pchannel ) );
      plcontrol->h.status = MAS_STATUS_OPEN;
      ro = r;
/* ?????? fcntl(fd, F_SETFD, FD_CLOEXEC) */
      MAS_LOG( "(%d) opened channel ========", r );
      OMSG( "INCOMING CONNECTION (%d)", ro );
    }
    if ( plcontrol->popts->thname.listenin )
      IEVAL( rn, prctl( PR_SET_NAME, ( unsigned long ) plcontrol->popts->thname.listenin /* "zocListenIn" */  ) );
    {
      struct timeval td;

      gettimeofday( &td, NULL );
      plcontrol->h.activity_time = td;
    }
    {
      CTRL_PREPARE;
      if ( r >= 0 && !ctrl.stop_listeners )
      { IEVAL( r, mas_listener_client_came( plcontrol ) );}
      else
      {
        MAS_LOG( "(%d) NOT opened? channel; opened : %d", r, plcontrol->h.pchannel->opened );
      }
    }
  }
  else
  {
#ifdef EMSG
    EMSG( "no plcontrol" );
#endif
    /* ctrl.keep_listening = 0; */
  }

  {
    CTRL_PREPARE;
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
  }
  return ro < 0 ? ( r > 0 ? r : 0 ) : r;
}
