#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include <sys/time.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;


#include <mastar/channel/mas_channel.h>
#include <mastar/channel/mas_channel_object.h>

#include <mastar/log/mas_log.h>

#include <mastar/types/mas_varset_types.h>
#include <mastar/varset/mas_varset_object.h>

#include "mas_lcontrol_object.h"

/*
this:
  mas_lcontrol_object.c
related:
  mas_listener_control.c
  mas_listener.c
  mas_rcontrol_object.c

  mas_channel_object.c
  mas_channel.c
  mas_control_types.h
*/

/*
this:
  mas_lcontrol_object.c
related:
  mas_lcontrol_object.h
  mas_listener_wait_client.c
more:
  mas_listener.c
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

  mas_cmdmod_listener.c

*/


mas_lcontrol_t *
mas_lcontrol_create( void )
{
  mas_lcontrol_t *plcontrol;

  plcontrol = mas_malloc( sizeof( mas_lcontrol_t ) );
  if ( plcontrol )
  {
    memset( plcontrol, 0, sizeof( mas_lcontrol_t ) );
    plcontrol->signature[0] = 'L';
    plcontrol->signature[1] = 'S';
  }
  else
  {
#ifdef EMSG
    EMSG( "FATAL" );
#endif
  }
  return plcontrol;
}

int
mas_lcontrol_register( mas_lcontrol_t * plcontrol )
{
  if ( ctrl.lcontrols_list )
  {
    /* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); */
    pthread_rwlock_wrlock( &ctrl.thglob.lcontrols_list_rwlock );
    MAS_LIST_ADD( ctrl.lcontrols_list, plcontrol, next );
    plcontrol->in_list = 1;
    plcontrol->h.serial = ++ctrl.listener_serial;
    MAS_LOG( "registering listener; serial:%lu", plcontrol->h.serial );

    pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
    /* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); */

    /* thMSG( "+C l/th %p ['%s':%u]", ( void * ) plcontrol, plcontrol->host, plcontrol->port ); */
  }
  else
  {
    /* ........... */
#ifdef EMSG
    EMSG( "+C l/th %p ['%s':%u]", ( void * ) plcontrol, plcontrol->host, plcontrol->port );
#endif
    sleep( 10 );
  }
  return 0;
}

/* naming : create + init + register = make */
mas_lcontrol_t *
mas_lcontrol_make( mas_options_t * popts, const char *host, unsigned port )
{
  mas_lcontrol_t *plcontrol = NULL;

  plcontrol = mas_lcontrol_create(  );
  if ( plcontrol )
  {
    mas_lcontrol_init( plcontrol, popts, host, port );
    mas_lcontrol_register( plcontrol );
  }
  else
  {
#ifdef EMSG
    EMSG( "?" );
#endif
  }
  return plcontrol;
}

static int
mas_lcontrol_create_transaction_controls_list( mas_lcontrol_t * plcontrol )
{
  if ( !plcontrol->transaction_controls_list )
  {
    /* WRONG pthread_mutex_lock( &plcontrol->transaction_mutex ); */
    /* WRONG pthread_rwlock_wrlock( &plcontrol->transaction_rwlock ); */
    plcontrol->transaction_controls_list = mas_malloc( sizeof( mas_rcontrol_list_head_t ) );
    if ( plcontrol->transaction_controls_list )
    {
      memset( plcontrol->transaction_controls_list, 0, sizeof( mas_rcontrol_list_head_t ) );
      MAS_LIST_INIT( plcontrol->transaction_controls_list );
    }
    /* WRONG pthread_rwlock_unlock( &plcontrol->transaction_rwlock ); */
    /* WRONG pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
  }
  return 0;
}

/* naming : set members = init */
int
mas_lcontrol_init( mas_lcontrol_t * plcontrol, mas_options_t * popts, const char *host, unsigned port )
{
  unsigned hport;

  plcontrol->popts = popts;
  plcontrol->hostlen = mas_parse_host_port( host, &hport, port );
  plcontrol->host = mas_strndup( host, plcontrol->hostlen );
  plcontrol->port = hport;
  /* thMSG( "HOST: %s; PORT:%u", plcontrol->host, plcontrol->port ); */
  plcontrol->h.thread = ( pthread_t ) 0;
  /* pthread_mutex_init( &plcontrol->transaction_mutex, NULL ); */
  pthread_rwlock_init( &plcontrol->transaction_rwlock, NULL );
  pthread_rwlock_init( &plcontrol->variables_rwlock, NULL );
  plcontrol->h.status = MAS_STATUS_NONE;

  plcontrol->h.pchannel = mas_channel_create(  );
  mas_channel_init( plcontrol->h.pchannel, ctrl.is_server, CHN_SOCKET, plcontrol->host, plcontrol->hostlen, plcontrol->port );
  {
    struct timeval td;

    gettimeofday( &td, NULL );
    plcontrol->h.activity_time = td;
  }
  mas_lcontrol_create_transaction_controls_list( plcontrol );
  return 0;
}

/* naming : free members + free = delete */
int
mas_lcontrol_remove_delete( mas_lcontrol_t * plcontrol )
{
  if ( plcontrol )
  {
    /* thMSG( "REMOVE %d %p", __LINE__, ( void * ) plcontrol ); */
    pthread_rwlock_wrlock( &ctrl.thglob.lcontrols_list_rwlock );
    if ( plcontrol->in_list && ctrl.lcontrols_list && !MAS_LIST_EMPTY( ctrl.lcontrols_list ) )
    {
      MAS_LIST_REMOVE( ctrl.lcontrols_list, plcontrol, mas_lcontrol_s, next );
      plcontrol->in_list = 0;
    }
    pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
    if ( plcontrol->host )
      mas_free( plcontrol->host );
    plcontrol->host = NULL;
    {
      mas_varset_t *vars;

      vars = plcontrol->variables;
      plcontrol->variables = NULL;
      mas_varset_delete( vars );
    }

    if ( plcontrol->h.pchannel )
    {
      mas_channel_delete( plcontrol->h.pchannel, 0, 0 );
      plcontrol->h.pchannel = NULL;
    }
    /* if ( plcontrol->transaction_controls_list && !MAS_LIST_EMPTY( plcontrol->transaction_controls_list ) ) */
    /* {                                                                                                      */
    /*   EMSG( "A transaction_controls_list!!!" );                                                            */
    /*   sleep( 2 );                                                                                          */
    /* }                                                                                                      */
    /* if ( plcontrol->transaction_controls_list && !MAS_LIST_EMPTY( plcontrol->transaction_controls_list ) ) */
    /* {                                                                                                      */
    /*   EMSG( "B transaction_controls_list!!!" );                                                            */
    /* }                                                                                                      */

    if ( plcontrol->transaction_controls_list )
    {
      /* pthread_mutex_lock( &plcontrol->transaction_mutex ); */
      pthread_rwlock_wrlock( &plcontrol->transaction_rwlock );
      mas_free( plcontrol->transaction_controls_list );
      plcontrol->transaction_controls_list = NULL;
      pthread_rwlock_unlock( &plcontrol->transaction_rwlock );
      /* pthread_mutex_unlock( &plcontrol->transaction_mutex ); */
    }
    /* pthread_mutex_destroy( &plcontrol->transaction_mutex ); */

    pthread_rwlock_destroy( &plcontrol->variables_rwlock );
    memset( &plcontrol->variables_rwlock, 0, sizeof( plcontrol->variables_rwlock ) );

    pthread_rwlock_destroy( &plcontrol->transaction_rwlock );
    memset( &plcontrol->transaction_rwlock, 0, sizeof( plcontrol->transaction_rwlock ) );

    /* memset( &plcontrol->transaction_mutex, 0, sizeof( plcontrol->transaction_mutex ) ); */

    /* thMSG( "FREE %d %p", __LINE__, ( void * ) plcontrol ); */
    plcontrol->h.thread = ( pthread_t ) 0;
    /* FIXME : double free or corruption (!prev) --- sometimes, on final destroy */
    mas_free( plcontrol );
  }
  return 0;
}
