#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>

#include <mastar/log/mas_log.h>

#include <mastar/thtools/mas_thread_tools.h>

#include <mastar/types/mas_varset_types.h>
#include <mastar/varset/mas_varset_search.h>

#include <mastar/autoset/mas_autoset_object.h>
#include <mastar/autoset/mas_autoset_search.h>
#include <mastar/autoobject/mas_autoobject.h>

#include <mastar/transaction/mas_rcontrol_object.h>

#include "mas_listener_control.h"

/*
this:
  mas_listener_control.c
related:
  mas_listener_control_list.c
  mas_control_types.h
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
  mas_ticker.c

  mas_cmdmod_listener.c
*/


/* mas_lcontrol_t *                                                  */
/* mas_lcontrol_create_array( size_t sz )                            */
/* {                                                                 */
/*   mas_lcontrol_t *plcontrols;                                     */
/*                                                                   */
/*   plcontrols = mas_calloc( sz + 1, sizeof( mas_lcontrol_t ) );    */
/*   memset( plcontrols, 0, ( sz + 1 ) * sizeof( mas_lcontrol_t ) ); */
/*                                                                   */
/*   return plcontrols;                                              */
/* }                                                                 */

/* mas_lcontrols_cleaning_transactions returns not-joined-count */
unsigned
mas_lcontrols_cleaning_transactions__( int removeit, long nanos )
{
  CTRL_PREPARE;
  int join_result = 0;
  mas_lcontrol_t *plcontrol = NULL;

  /* MAS_LOG( "all listeners cleaning tr." ); */
  if ( ctrl.lcontrols_list && !MAS_LIST_EMPTY( ctrl.lcontrols_list ) )
  {
    MAS_LIST_FOREACH( plcontrol, ctrl.lcontrols_list, next )
    {
      int jr;

/* mas_lcontrol_cleaning_transactions__ returns not-joined-count */
      jr = mas_lcontrol_cleaning_transactions__( plcontrol, removeit, nanos );
      /* if ( jr > 0 )                                                                 */
      /* {                                                                             */
      /*   MAS_LOG( "listener #%lu cleaned ; remains: %d tr.", plcontrol->h.serial, jr ); */
      /* }                                                                             */
      join_result += jr;
    }
  }
  else
  {
    /* thMSG( "empty lcontrols_list" ); */
  }
  /* if ( join_result > 0 )                                                       */
  /* {                                                                            */
  /*   MAS_LOG( "all listeners cleaned transactions - remains %u", join_result ); */
  /* }                                                                            */
  return join_result;
}

static void
mas_u_cleanup( void *arg )
{
  CTRL_PREPARE;
  pthread_mutex_unlock( &ctrl.thglob.cleanup_transactions_mutex );
}

/* mas_lcontrol_cleaning_transactions__ returns not-joined-count */
unsigned
mas_lcontrol_cleaning_transactions__( mas_lcontrol_t * plcontrol, int removeit, long nanos )
{
  int join_result = 0;

  if ( plcontrol )
  {
    CTRL_PREPARE;
    unsigned rmcnt = 0;

    MAS_LOG( "cleanup transactions; to lock" );
    pthread_cleanup_push( mas_u_cleanup, NULL );
    pthread_mutex_lock( &ctrl.thglob.cleanup_transactions_mutex );
    MAS_LOG( "cleanup transactions; locked" );
    if ( plcontrol->transaction_controls_list && !MAS_LIST_EMPTY( plcontrol->transaction_controls_list ) )
    {
      mas_rcontrol_t *prcontrol = NULL;
      mas_rcontrol_t *prcontrol_next = NULL;

      /* mas_rcontrol_t *prcontrol_tmp = NULL; */

      /* while ( plcontrol && ( prcontrol = MAS_LIST_FIRST( plcontrol->transaction_controls_list ) ) ) */
      /* pthread_rwlock_rdlock( &plcontrol->transaction_rwlock ); */

      /* MAS_LOG( "to clean transactions list (nanos:%lu)", nanos ); */

      for ( prcontrol = MAS_LIST_FIRST( plcontrol->transaction_controls_list ); prcontrol; prcontrol = prcontrol_next )
        /* MAS_LIST_FOREACH_SAFE( prcontrol, plcontrol->transaction_controls_list, next, prcontrol_tmp ) */
      {
        /* thMSG( "to join r/th %lx to %lx", ( unsigned long ) mas_pthread_self(  ), ( unsigned long ) prcontrol->h.thread ); */
        if ( prcontrol->h.thread )
        {
          prcontrol_next = MAS_LIST_NEXT( prcontrol, next );
//        if ( nanos >= 0 )
//        {
//          thMSG( "waiting (%lu) cl. R%lu:%u @ L%lu:%u to go (server quit)", nanos, prcontrol->h.serial, prcontrol->h.status,
//                 plcontrol->h.serial, plcontrol->h.status );
//        }
          {
            int j;

            if ( 0 )
            {
              /* TEST */
              /* int rcond; */

              if ( !( prcontrol->h.pchannel && prcontrol->h.pchannel->opened ) )
              {
                ( void ) /* rcond = */ pthread_cond_signal( &prcontrol->waitchan_cond );
                /* thMSG( "COND_SIGNAL :%d", rcond ); */
              }
            }
            /* j = Jwait ? mas_xpthread_join( prcontrol->h.thread ) : mas_xpthread_tryjoin_np( prcontrol->h.thread ); */
            j = mas_xxpthread_join( prcontrol->h.thread, nanos );
            if ( 0 == j )
            {
              /* MAS_LOG( "stoped transaction [%lx]", prcontrol->h.thread ); */
              prcontrol->h.thread = ( pthread_t ) 0;
              rmcnt++;
              /* thMSG( "joined L%lu:%u & R%lu:%u", plcontrol->h.serial, plcontrol->h.status, prcontrol->h.serial, prcontrol->h.status ); */
              MAS_LOG( "stopped  R%lu:%u @ L%lu:%u; rmcnt:%u (to wait %lu) 0x%lx", prcontrol->h.serial, prcontrol->h.status,
                       plcontrol->h.serial, plcontrol->h.status, rmcnt, nanos, prcontrol->h.thread );
            }
            else
            {
              /* if ( j )                         */
              /* {                                */
              /*   MAS_LOGERR( j, "not joined" ); */
              /* }                                */
              join_result++;
            }
          }
        }
        else
        {
          EMSG( "tr.th not set at list - BAD" );
        }
        /* if ( prcontrol && !prcontrol->h.thread && removeit )                                            */
        /* {                                                                                               */
        /*   MAS_LOG( "to delete transaction" );                                                           */
        /*   mas_rcontrol_delete( prcontrol, 0 );                                                          */
        /* }                                                                                               */
        /* else                                                                                            */
        /* {                                                                                               */
        /*   (* EMSG( "not del.tr (%d:%d) - BAD", removeit, prcontrol && prcontrol->h.thread ? 1 : 0 ); *) */
        /* }                                                                                               */
        if ( !plcontrol->transaction_controls_list || MAS_LIST_EMPTY( plcontrol->transaction_controls_list ) )
          break;
      }
      /* pthread_rwlock_unlock( &plcontrol->transaction_rwlock ); */
    }
    pthread_mutex_unlock( &ctrl.thglob.cleanup_transactions_mutex );
    pthread_cleanup_pop( 0 );
    MAS_LOG( "cleanup transactions; unlocked" );
    if ( rmcnt > 0 )
    {
      MAS_LOG( "cleanup %d transactions; unlocked; join_result=%u", rmcnt, join_result );
    }
  }
  else
  {
    /* thMSG( "no plcontrol" ); */
  }
  return join_result;
}

/* unsigned                                                                                                 */
/* mas_lcontrol_cleaning_transactions_count( mas_lcontrol_t * plcontrol, int removeit, long Jwait )         */
/* {                                                                                                        */
/*   int cnt = 0;                                                                                           */
/*   mas_rcontrol_t *prcontrol;                                                                             */
/*                                                                                                          */
/*   (* while ( 0 == mas_lcontrol_cleaning_transactions__( plcontrol, removeit, Jwait ) ); *)                  */
/*   mas_lcontrol_cleaning_transactions__( plcontrol, removeit, Jwait );                                       */
/*   if ( plcontrol->transaction_controls_list && !MAS_LIST_EMPTY( plcontrol->transaction_controls_list ) ) */
/*   {                                                                                                      */
/*     (* pthread_mutex_lock( &plcontrol->transaction_mutex ); *)                                           */
/*     pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );                                             */
/*     MAS_LIST_FOREACH( prcontrol, plcontrol->transaction_controls_list, next )                            */
/*     {                                                                                                    */
/*       cnt++;                                                                                             */
/*     }                                                                                                    */
/*     pthread_rwlock_unlock( &plcontrol->transaction_rwlock );                                             */
/*     (* pthread_mutex_unlock( &plcontrol->transaction_mutex ); *)                                         */
/*   }                                                                                                      */
/*   return cnt;                                                                                            */
/* }                                                                                                        */


int
mas_lcontrol_variable_create_text( mas_lcontrol_t * plcontrol, /* th_type_t thtype, */ const char *vclass_name, const char *name,
                                   const char *txt )
{
  int r = 0;

  if ( plcontrol && name )
  {
    pthread_rwlock_wrlock( &plcontrol->variables_rwlock );
    plcontrol->variables = mas_varset_search_variable( plcontrol->variables, vclass_name, name, txt );
    pthread_rwlock_unlock( &plcontrol->variables_rwlock );
  }
  return r;
}

int
mas_lcontrol_variable_set_text( mas_lcontrol_t * plcontrol, /* th_type_t thtype, */ const char *vclass_name, const char *name,
                                const char *txt )
{
  int r = 0;

  if ( plcontrol && name )
  {
    pthread_rwlock_wrlock( &plcontrol->variables_rwlock );
    plcontrol->variables = mas_varset_search_variable( plcontrol->variables, vclass_name, name, txt );
    pthread_rwlock_unlock( &plcontrol->variables_rwlock );
  }
  return r;
}

mas_varvec_element_t *
mas_lcontrol_variables_find( mas_lcontrol_t * plcontrol, const char *vclass_name, const char *name )
{
  mas_varvec_element_t *found = NULL;

  if ( plcontrol && name )
  {
    pthread_rwlock_rdlock( &plcontrol->variables_rwlock );
    found = mas_varset_find_variable( plcontrol->variables, vclass_name, name );

    pthread_rwlock_unlock( &plcontrol->variables_rwlock );
  }
  return found;
}

mas_autoobject_t *
mas_lcontrol_get_autoobject( mas_lcontrol_t * plcontrol, const char *docroot, const char *uri )
{
  mas_autoobject_t *ao = NULL;

  pthread_mutex_lock( &plcontrol->autoset_mutex );
  if ( !plcontrol->autoset )
    plcontrol->autoset = mas_autoset_create(  );
  if ( plcontrol->autoset )
  {
    ao = mas_autoset_search_autoobject( plcontrol->autoset, docroot, uri );
    mas_autoobject_set_iaccess_type( ao, MAS_IACCESS_SENDFILE );
  }
  pthread_mutex_unlock( &plcontrol->autoset_mutex );

  return ao;
}
