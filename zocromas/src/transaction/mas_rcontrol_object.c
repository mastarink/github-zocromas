#include "mas_basic_def.h"

#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/channel/mas_channel_object.h>
#include <mastar/channel/mas_channel.h>

#include "mas_common.h"
#include "log/inc/mas_log.h"

#include "transaction/inc/mas_transaction_control.h"
#include "transaction/inc/mas_rcontrol_object.h"


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
  
  mas_lib_thread.c

*/



mas_rcontrol_t *
mas_rcontrol_create( void )
{
  mas_rcontrol_t *prcontrol;

  prcontrol = mas_malloc( sizeof( mas_rcontrol_t ) );
  memset( prcontrol, 0, sizeof( mas_rcontrol_t ) );
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
    if ( plcontrol->pchannel && plcontrol->pchannel->opened )
    {
      mas_rcontrol_set_channel( prcontrol, plcontrol->pchannel );
      /* prcontrol->pchannel = mas_channel_clone( plcontrol->pchannel ); */
      /* prcontrol->serv = prcontrol->pchannel->serv;               */
      /* prcontrol->port = prcontrol->pchannel->port;                    */
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
      prcontrol->activity_time = td;
    }
  }
  else
  {
    EMSG( "FATAL: no plcontrol" );
  }
  return 0;
}

int
mas_rcontrol_register( mas_rcontrol_t * prcontrol, mas_lcontrol_t * plcontrol )
{
  if ( plcontrol->transaction_controls_list )
  {
    /* thMSG( "ADD %lu %lx", prcontrol->serial, prcontrol->thread ); */
    /* pthread_mutex_lock( &plcontrol->transaction_mutex ); */
    pthread_rwlock_wrlock( &plcontrol->transaction_rwlock );
    {
      MAS_LIST_ADD( plcontrol->transaction_controls_list, prcontrol, next );
      prcontrol->serial = ++plcontrol->transaction_serial;
      MAS_LOG( "registering transaction; serial:%lu", prcontrol->serial );
    }
    pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
    /* pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
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
    /* EMSG( "!!!!!!!! FATAL: R%lu:%u @ L%lu:%u NOT complete", prcontrol->serial, prcontrol->status, plcontrol->serial, */
    /*       plcontrol->status );                                                                                       */
    prcontrol->nc++;
  }
  else
  {
    pthread_mutex_destroy( &prcontrol->waitchan_mutex );
    pthread_cond_destroy( &prcontrol->waitchan_cond );
    if ( prcontrol->nc )
    {
      MAS_LOG( "rm with nc=%u R%lu:%u @ L%lu:%u NOT complete", prcontrol->nc, prcontrol->serial, prcontrol->status,
               plcontrol->serial, plcontrol->status );
    }
    else
    {
      /* thMSG( "rm R%lu:%u @ L%lu:%u", prcontrol->serial, prcontrol->status, plcontrol->serial, plcontrol->status ); */
      MAS_LOG( "rm R%lu:%u @ L%lu:%u", prcontrol->serial, prcontrol->status, plcontrol->serial, plcontrol->status );
    }
    if ( prcontrol->uuid )
      mas_free( prcontrol->uuid );
    prcontrol->uuid = NULL;

    if ( prcontrol->pchannel )
    {
      mas_channel_t *pchannel = NULL;

      pchannel = prcontrol->pchannel;
      prcontrol->pchannel = NULL;
      mas_channel_delete( pchannel, toclose /* close */ , 0 );
    }

    /* pthread_mutex_lock( &plcontrol->transaction_mutex ); */
    pthread_rwlock_wrlock( &plcontrol->transaction_rwlock );
    {
      MAS_LIST_REMOVE( plcontrol->transaction_controls_list, prcontrol, mas_rcontrol_s, next );
      MAS_LOG( "removed transaction from list" );
    }
    pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
    /* pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
    /* thMSG( "RM %lu %lx", prcontrol->serial, prcontrol->thread ); */
    mas_free( prcontrol );
  }
  return 0;
}
