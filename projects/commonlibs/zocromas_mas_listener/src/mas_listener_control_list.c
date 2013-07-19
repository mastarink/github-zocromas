#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdlib.h>
#include <pthread.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;

#include <mastar/log/mas_log.h>

#include "mas_lcontrol_object.h"
#include "mas_listener_control_list.h"

/*
this:
  mas_listener_control_list.c
related:
  mas_control_types.h
  mas_listener_control.c
  mas_listener_control.h
  mas_lcontrol_object.c
  mas_lcontrol_object.h
more:
  mas_listener.c
  mas_listener_wait_client.c
  mas_listener_control_list.h
  mas_listener_control_types.h
  mas_listener.h
  mas_listeners.c
  mas_listeners.h
  mas_listener_wait_client.h
  mas_server_main.c

  mas_cmdmod_listener.c
*/

int
mas_lcontrols_init( mas_options_t * popts, const char * *message )
{
  HMSG( "INIT LCONTROLS" );
  mas_lcontrols_list_create(  );
  if ( message )
    *message = __func__;
  return ctrl.lcontrols_list ? 0 : -1;
}

void
mas_lcontrols_list_create( void )
{
  HMSG( "INIT LCONTROLS_LIST" );
  ctrl.lcontrols_list = mas_malloc( sizeof( mas_lcontrol_list_head_t ) );
  memset( ctrl.lcontrols_list, 0, sizeof( mas_lcontrol_list_head_t ) );
  MAS_LIST_INIT( ctrl.lcontrols_list );
}

int
mas_lcontrols_clean_list( int force )
{
  int cleaned = 0;

  if ( ctrl.lcontrols_list && !MAS_LIST_EMPTY( ctrl.lcontrols_list ) )
  {
    mas_lcontrol_t *plcontrol = NULL, *plcontrol_next = NULL;

    /* mas_lcontrol_t *plcontrol_tmp; */

    /* mas_nanosleep( 2.5 ); */
    /* EMSG( "@@@@@@@@@" );  */
    MAS_LOG( "to clean listeners list" );
    for ( plcontrol = MAS_LIST_FIRST( ctrl.lcontrols_list ); plcontrol; plcontrol = plcontrol_next )
      /* MAS_LIST_FOREACH_SAFE( plcontrol, ctrl.lcontrols_list, next, plcontrol_next ) */
    {
      plcontrol_next = MAS_LIST_NEXT( plcontrol, next );
      MAS_LOG( "to remove listener plc %p from list [%lx] (force:%d)", ( void * ) plcontrol, plcontrol->h.thread, force );
      if ( force || !plcontrol->h.thread )
      {
        if ( plcontrol->h.thread )
        {
#ifdef EMSG
          EMSG( "WHY thread is not 0 (%lx)", plcontrol->h.thread );
#endif
        }

        /* thMSG( "L DELETE %d %p", __LINE__, ( void * ) plcontrol ); */
        /* naming : free members + free = delete */

        /* FIXME : double free or corruption (!prev) */
        if ( mas_lcontrol_remove_delete( plcontrol ) == 0 )
          cleaned++;
        plcontrol = NULL;
      }
    }
  }
  if ( cleaned )
  {
    HMSG( "CLEANED %d lcontrols", cleaned );
  }
  return cleaned;
}

void
mas_lcontrols_delete_list( void )
{
  int cleaned;

  cleaned = mas_lcontrols_clean_list( 1 );
  if ( cleaned )
  {
    HMSG( "DEL.LIST : CLEANED %d lcontrols", cleaned );
  }
  if ( ctrl.lcontrols_list && MAS_LIST_EMPTY( ctrl.lcontrols_list ) )
  {
    /* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); */
    pthread_rwlock_wrlock( &ctrl.thglob.lcontrols_list_rwlock );

    mas_free( ctrl.lcontrols_list );
    ctrl.lcontrols_list = NULL;

    pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
    /* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); */
  }
}

/* find by host + port */
mas_lcontrol_t *
mas_lcontrol_find( const char *host, int port )
{
  mas_lcontrol_t *plcontrol = NULL, *pl;

  if ( ctrl.lcontrols_list && !MAS_LIST_EMPTY( ctrl.lcontrols_list ) )
  {
    /* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); */
    pthread_rwlock_rdlock( &ctrl.thglob.lcontrols_list_rwlock );

    MAS_LIST_FOREACH( pl, ctrl.lcontrols_list, next )
    {
      /* thMSG( "pl: %p; '%s':%u", ( void * ) pl, pl->host, pl->port ); */
      if ( pl && pl->port == port && 0 == strcmp( pl->host, host ) )
      {
        plcontrol = pl;
        break;
      }
    }

    pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );
    /* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); */
  }
  return plcontrol;
}
