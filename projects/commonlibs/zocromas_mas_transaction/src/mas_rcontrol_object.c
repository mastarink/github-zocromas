#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include <pthread.h>

#include <mastar/types/mas_control_types.h>
/* #include <mastar/types/mas_opts_types.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/channel/mas_channel_object.h>
#include <mastar/channel/mas_channel.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include "mas_transaction_control.h"
#include "mas_rcontrol_object.h"


/*
this:
  mas_rcontrol_object.h
related:
  mas_transaction.c
  mas_transaction.h
  mas_transaction_control.c
  mas_transaction_control_types.h
  mas_master.c
  mas_client.c
  mas_transaction_xcromas.c
  mas_transaction_http.c
  
  mas_listener_control_types.h
  mas_control.c
*/



mas_rcontrol_t *
mas_rcontrol_create( void )
{
  mas_rcontrol_t *prcontrol;

  prcontrol = mas_malloc( sizeof( mas_rcontrol_t ) );
  if ( prcontrol )
  {
    CTRL_PREPARE;
    memset( prcontrol, 0, sizeof( mas_rcontrol_t ) );
    prcontrol->c.signature[0] = 'T';
    prcontrol->c.signature[1] = 'R';
    ctrl.clients_created++;
  }
  return prcontrol;
}

#ifdef MAS_TR_PERSIST
int
mas_rcontrol_init( mas_rcontrol_t * prcontrol, mas_lcontrol_t * plcontrol, unsigned persistent_transaction )
#else
int
mas_rcontrol_init( mas_rcontrol_t * prcontrol, mas_lcontrol_t * plcontrol )
#endif
{
  MAS_LOG( "init transaction" );
  if ( plcontrol )
  {
    if ( plcontrol->h.pchannel && plcontrol->h.pchannel->opened )
    {
      mas_rcontrol_set_channel( prcontrol, plcontrol->h.pchannel );
      /* prcontrol->h.pchannel = mas_channel_clone( plcontrol->h.pchannel ); */
      /* prcontrol->serv = prcontrol->h.pchannel->serv;               */
      /* prcontrol->port = prcontrol->h.pchannel->port;                    */
    }
#ifdef MAS_TR_PERSIST
    prcontrol->persistent_transaction = persistent_transaction;
#endif
    prcontrol->plcontrol = plcontrol;
    pthread_cond_init( &prcontrol->waitchan_cond, NULL );
    pthread_mutex_init( &prcontrol->waitchan_mutex, NULL );
    {
      struct timeval td;

      gettimeofday( &td, NULL );
      prcontrol->h.activity_time = td;
    }
  }
  else
  {
#ifdef EMSG
    EMSG( "FATAL: no plcontrol" );
#endif
  }
  return 0;
}

int
mas_rcontrol_register( mas_rcontrol_t * prcontrol, mas_lcontrol_t * plcontrol )
{
  if ( plcontrol->transaction_controls_list )
  {

    /* thMSG( "ADD %lu %lx", prcontrol->h.serial, prcontrol->h.thread ); */
    {
      unsigned long ser = 0;

      /* pthread_mutex_lock( &plcontrol->transaction_mutex ); */
      pthread_rwlock_wrlock( &plcontrol->transaction_rwlock );
      {
        MAS_LIST_ADD( plcontrol->transaction_controls_list, prcontrol, next );
        prcontrol->h.serial = ++plcontrol->transaction_serial;
        ser = prcontrol->h.serial;
      }
      pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
      /* pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
      MAS_LOG( "registering transaction; serial:%lu", ser );
    }
  }
  return 0;
}

/* naming : create + init + register = make */
#ifdef MAS_TR_PERSIST
mas_rcontrol_t *
mas_rcontrol_make( mas_lcontrol_t * plcontrol, unsigned persistent_transaction )
#else
mas_rcontrol_t *
mas_rcontrol_make( mas_lcontrol_t * plcontrol )
#endif
{
  mas_rcontrol_t *prcontrol;

  MAS_LOG( "make transaction" );
  prcontrol = mas_rcontrol_create(  );

  if ( prcontrol )
  {
#ifdef MAS_TR_PERSIST
    mas_rcontrol_init( prcontrol, plcontrol, persistent_transaction );
#else
    mas_rcontrol_init( prcontrol, plcontrol );
#endif
    mas_rcontrol_register( prcontrol, plcontrol );
  }
  return prcontrol;
}

/* naming : free members + free = delete */
int
mas_rcontrol_delete( mas_rcontrol_t * prcontrol, int toclose )
{
  mas_lcontrol_t *plcontrol = NULL;

  plcontrol = prcontrol->plcontrol;
  if ( !prcontrol->complete )
  {
    EMSG( "!!!!!!!! FATAL: R%lu:%u @ L%lu:%u NOT complete", prcontrol->h.serial, prcontrol->c.status, plcontrol->h.serial,
          plcontrol->c.status );
    prcontrol->nc++;
  }
  else
  {
    pthread_mutex_destroy( &prcontrol->waitchan_mutex );
    pthread_cond_destroy( &prcontrol->waitchan_cond );
    if ( prcontrol->nc )
    {
      MAS_LOG( "rm with nc=%u R%lu:%u @ L%lu:%u NOT complete", prcontrol->nc, prcontrol->h.serial, prcontrol->c.status,
               plcontrol->h.serial, plcontrol->c.status );
    }
    else
    {
      /* thMSG( "rm R%lu:%u @ L%lu:%u", prcontrol->h.serial, prcontrol->c.status, plcontrol->h.serial, plcontrol->c.status ); */
      MAS_LOG( "rm R%lu:%u @ L%lu:%u", prcontrol->h.serial, prcontrol->c.status, plcontrol->h.serial, plcontrol->c.status );
    }
    if ( prcontrol->uuid )
      mas_free( prcontrol->uuid );
    prcontrol->uuid = NULL;

    if ( prcontrol->h.pchannel )
    {
      mas_channel_t *pchannel = NULL;

      pchannel = prcontrol->h.pchannel;
      prcontrol->h.pchannel = NULL;
      mas_channel_delete( pchannel, toclose /* close */ , 0 );
    }

    {
      /* pthread_mutex_lock( &plcontrol->transaction_mutex ); */
      pthread_rwlock_wrlock( &plcontrol->transaction_rwlock );
      {
        CTRL_PREPARE;
        MAS_LIST_REMOVE( plcontrol->transaction_controls_list, prcontrol, mas_rcontrol_s, next );
        ctrl.clients_removed++;
      }
      pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
      MAS_LOG( "removed transaction from list" );
      /* pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
    }
    /* thMSG( "RM %lu %lx", prcontrol->h.serial, prcontrol->h.thread ); */
    mas_free( prcontrol );
  }
  return 0;
}
