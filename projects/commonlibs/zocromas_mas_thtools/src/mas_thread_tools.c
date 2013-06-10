#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>

#include <mastar/types/mas_control_types.h>
extern mas_control_t ctrl;


#include "mas_thread_tools.h"


/*
this:
  mas_thread_tools.c
related:
  mas_thread_variables.c
more:
  mas_init_threads.c
  mas_init_threads.h
  mas_thread_variables_types.h
  mas_thread_variables.h
  mas_lib_thread.h
  mas_thread_tools.h
  mas_lib_thread.c

*/

const char *
mas_thread_self_type_name( void )
{
  return mas_thread_type_name( mas_thself_type(  ) );
}

const char *
mas_thread_type_name( th_type_t typ )
{
  const char *s = NULL;

  switch ( typ )
  {
  case MAS_THREAD_MAIN:
    s = "MAIN";
    break;
  case MAS_THREAD_MASTER:
    s = "MAST";
    break;
  case MAS_THREAD_LISTENER:
    s = "LIST";
    break;
  case MAS_THREAD_TRANSACTION:
    s = "TRAN";
    break;
  case MAS_THREAD_TICKER:
    s = "TICK";
    break;
  case MAS_THREAD_WATCHER:
    s = "WATC";
    break;
  case MAS_THREAD_LOGGER:
    s = "LOGG";
    break;
  default:
    s = "NONE";
    break;
  }
  return s;
}

/* TODO move to listener_control.c
 * see: mas_lcontrol_find( const char *host, int port )
 * */
/* mas_lcontrol_t *                                                      */
/* mas_find_listener_control( pthread_t pth, int *ppos )                  */
/* {                                                                      */
/*   mas_lcontrol_t *plcontrol = NULL;                                   */
/*   int pos = 0;                                                         */
/*   mas_lcontrol_t *plc;                                                */
/*                                                                        */
/*   if ( ctrl.lcontrols_list && !MAS_LIST_EMPTY( ctrl.lcontrols_list ) ) */
/*   {                                                                    */
/*     (* pthread_mutex_lock( &ctrl.thglob.lcontrols_list_mutex ); *)     */
/*     pthread_rwlock_rdlock( &ctrl.thglob.lcontrols_list_rwlock );       */
/*     MAS_LIST_FOREACH( plc, ctrl.lcontrols_list, next )                 */
/*     {                                                                  */
/*       if ( pthread_equal( pth, plc->thread ) )                         */
/*       {                                                                */
/*         plcontrol = plc;                                               */
/*         if ( ppos )                                                    */
/*           *ppos = pos;                                                 */
/*         break;                                                         */
/*       }                                                                */
/*       pos++;                                                           */
/*     }                                                                  */
/*     pthread_rwlock_unlock( &ctrl.thglob.lcontrols_list_rwlock );       */
/*     (* pthread_mutex_unlock( &ctrl.thglob.lcontrols_list_mutex ); *)   */
/*                                                                        */
/*     if ( !plcontrol )                                                  */
/*     {                                                                  */
/*       (* MFP( "\n\n\nNot found l/thread %lx\n\n\n", pth ); *)          */
/*     }                                                                  */
/*     if ( ppos )                                                        */
/*       *ppos = pos;                                                     */
/*   }                                                                    */
/*   return plcontrol;                                                    */
/* }                                                                      */

/* mas_rcontrol_t *                                                                                           */
/* mas_find_transaction_control( mas_lcontrol_t * plcontrol, pthread_t pth, int *ppos )                      */
/* {                                                                                                          */
/*   mas_rcontrol_t *prcontrol = NULL;                                                                        */
/*                                                                                                            */
/*   if ( ppos )                                                                                              */
/*     *ppos = -2000;                                                                                         */
/*   if ( plcontrol )                                                                                         */
/*   {                                                                                                        */
/*     mas_rcontrol_t *prc = NULL;                                                                            */
/*                                                                                                            */
/*     if ( ppos )                                                                                            */
/*       *ppos = -2010;                                                                                       */
/*                                                                                                            */
/*     if ( plcontrol->transaction_controls_list && !MAS_LIST_EMPTY( plcontrol->transaction_controls_list ) ) */
/*     {                                                                                                      */
/*       long pos = 0;                                                                                        */
/*                                                                                                            */
/*       (* pthread_mutex_lock( &plcontrol->transaction_mutex ); *)                                           */
/*       pthread_rwlock_rdlock( &plcontrol->transaction_rwlock );                                             */
/*       MAS_LIST_FOREACH( prc, plcontrol->transaction_controls_list, next )                                  */
/*       {                                                                                                    */
/*         if ( pthread_equal( pth, prc->thread ) )                                                           */
/*         {                                                                                                  */
/*           prcontrol = prc;                                                                                 */
/*           if ( ppos )                                                                                      */
/*             *ppos = pos;                                                                                   */
/*           break;                                                                                           */
/*         }                                                                                                  */
/*         pos++;                                                                                             */
/*       }                                                                                                    */
/*       pthread_rwlock_unlock( &plcontrol->transaction_rwlock );                                             */
/*       (* pthread_mutex_unlock( &plcontrol->transaction_mutex ); *)                                         */
/*     }                                                                                                      */
/*   }                                                                                                        */
/*   else                                                                                                     */
/*   {                                                                                                        */
/*     EMSG( "FATAL" );                                                                                       */
/*   }                                                                                                        */
/*   return prcontrol;                                                                                        */
/* }                                                                                                          */


static void
mas_delete_thread_specific( void *ptr )
{
  mas_thdata_t *thd;

  thd = ( mas_thdata_t * ) ptr;
  if ( thd )
  {
    /* HMSG( "IN THREAD END [%lx] (self) %s", pthread_self(  ), mas_thread_type_name( thd->type ) ); */
  }
  mas_free( ptr );
}

void
mas_thread_make_key( void )
{
  ( void ) pthread_key_create( &ctrl.mas_thread_key, mas_delete_thread_specific );
}

void
mas_in_thread_end( void )
{
  mas_thdata_t *thd;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  thd = pthread_getspecific( ctrl.mas_thread_key );
  mas_delete_thread_specific( thd );
  ( void ) pthread_setspecific( ctrl.mas_thread_key, NULL );
}

mas_thdata_t *
mas_in_thread( th_type_t thtype, mas_lcontrol_t * plcontrol, mas_rcontrol_t * prcontrol /* , mas_channel_t * pchannel */  )
{
  mas_thdata_t *thd = NULL;

  pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  if ( ( thd = pthread_getspecific( ctrl.mas_thread_key ) ) == NULL )
  {
    thd = mas_malloc( sizeof( mas_thdata_t ) );
    memset( thd, 0, sizeof( mas_thdata_t ) );

    thd->type = thtype;
    /* thd->pchannel = pchannel; */
    thd->plcontrol = plcontrol;
    thd->prcontrol = prcontrol;
    ( void ) pthread_setspecific( ctrl.mas_thread_key, thd );
    /* HMSG( "IN THREAD [%lx] (self) %s", pthread_self(  ), mas_thread_self_type_name(  ) ); */
  }
  else
  {
    /* HMSG( "IN THREAD NO [%lx] (self) %s", pthread_self(  ), mas_thread_self_type_name(  ) ); */
  }
  return thd;
}

th_type_t
mas_thself_type( void )
{
  mas_thdata_t *thd;
  th_type_t thtype = MAS_THREAD_NONE;

  if ( ctrl.threads.n.main.thread && pthread_equal( ctrl.threads.n.main.thread, pthread_self(  ) ) )
  {
    thtype = MAS_THREAD_MAIN;
  }
  else
  {
    ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
    thd = pthread_getspecific( ctrl.mas_thread_key );
    if ( thd )
    {
      thtype = thd->type;
      /* thMSG( "%u : [%lx]", thtype, pthread_self(  ) ); */
    }
  }
  return thtype;
}

mas_lcontrol_t *
mas_thself_plcontrol( void )
{
  mas_thdata_t *thd;
  mas_lcontrol_t *plcontrol = NULL;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  thd = pthread_getspecific( ctrl.mas_thread_key );
  if ( thd )
  {
    plcontrol = thd->plcontrol;
  }
  return plcontrol;
}

double
mas_thself_double_time( void )
{
  double double_time;
  mas_thdata_t *thd;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  thd = pthread_getspecific( ctrl.mas_thread_key );
  if ( thd )
  {
    double_time = thd->double_time;
  }
  return double_time;
}

void
mas_thself_set_double_time( double double_time )
{
  mas_thdata_t *thd;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  thd = pthread_getspecific( ctrl.mas_thread_key );
  if ( thd )
  {
    thd->double_time = double_time;
  }
}

mas_rcontrol_t *
mas_thself_prcontrol( void )
{
  mas_thdata_t *thd;
  mas_rcontrol_t *prcontrol = NULL;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  thd = pthread_getspecific( ctrl.mas_thread_key );
  if ( thd )
  {
    prcontrol = thd->prcontrol;
  }
  return prcontrol;
}

mas_channel_t *
mas_thself_pchannel( void )
{
  mas_thdata_t *thd;
  mas_channel_t *pchannel = NULL;

  ( void ) pthread_once( &ctrl.mas_thread_key_once, mas_thread_make_key );
  thd = pthread_getspecific( ctrl.mas_thread_key );
  if ( thd )
  {
    if ( thd->prcontrol )
      pchannel = thd->prcontrol->h.pchannel;
    else if ( thd->plcontrol )
      pchannel = thd->plcontrol->h.pchannel;
  }
  return pchannel;
}

int
mas_xxpthread_join( pthread_t thread, long nanos )
{
  return mas_xxspthread_join( thread, 0, nanos );
}

int
mas_xxspthread_join( pthread_t thread, long sec, long nanos )
{
  struct timespec t;

  t.tv_sec = sec;
  t.tv_nsec = nanos;
  if ( nanos < 0 )
    /* wait forever */ return pthread_join( thread, NULL );
  else if ( sec == 0 && nanos == 0 )
    /* don't wait */ return pthread_tryjoin_np( thread, NULL );
  else
    /* wnait nanoseconds */ return mas_xxtspthread_join( thread, &t );
}

int
mas_xxtspthread_join( pthread_t thread, struct timespec *jwait )
{
  int r = -1;

  {
    /* wait n nanosecs */
    struct timespec t, ts;
    struct timeval td;

    gettimeofday( &td, NULL );
    ts.tv_sec = td.tv_sec;
    ts.tv_nsec = td.tv_usec * 1000;

    {
      t.tv_sec = ts.tv_sec + jwait->tv_sec;
      t.tv_nsec = ts.tv_nsec + jwait->tv_nsec;
      if ( t.tv_nsec >= 1000000000L )
      {
        t.tv_sec++;
        t.tv_nsec = t.tv_nsec - 1000000000L;
      }
    }

    /* absolute time measured since the Epoch !!!!!!!!!! */
    {
      /* r = mas_xpthread_timedjoin_np( thread, &t ); */
      r = pthread_timedjoin_np( thread, NULL, &t );
// ETIMEDOUT 110 /* Connection timed out */
#ifdef MAS_LOGERR
      MAS_LOGERR( r, "timed join [%lx] : %ld.%-ld => %d", thread, jwait->tv_sec, jwait->tv_nsec, r );
#endif
    }
  }
  return r;
}
