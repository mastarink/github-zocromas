#include <mastar/wrap/mas_std_def.h>

/* #define MS_DUP_FUNC_NAME */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef MAS_NO_THREADS
#  include <pthread.h>
#endif

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */

#include <mastar/thtools/mas_thread_tools.h>

#include "mas_logger.h"
#include "mas_log.h"

/*
this:
  mas_log.c
related:
  mas_logger.c
more:
  mas_log_types.h
  mas_log.h
  mas_logger.h

  mas_control_types.h
  mas_control_data.c

*/


void
mas_log_delete_loginfo( mas_loginfo_t * li )
{
  if ( li )
  {
    if ( li->message )
      mas_free( li->message );
    li->message = NULL;
#ifdef MS_DUP_FUNC_NAME
    if ( li->func )
      mas_free( li->func );
#endif
    mas_free( li );
  }
}

void
mas_log_clean_queue( void )
{
  mas_loginfo_t *li = NULL;
  mas_loginfo_list_head_t *log_list;

  log_list = mas_logger_list( 0 );
  ctrl.keep_logging = 0;
  while ( log_list && !MAS_LIST_EMPTY( log_list ) && ( li = MAS_LIST_FIRST( log_list ) ) )
  {
#ifndef MAS_NO_THREADS
    /* mas_pthread_mutex_lock( &logger_queue_mutex ); */
    pthread_rwlock_wrlock( &logger_queue_rwlock );
#endif
    MAS_LIST_REMOVE_HEAD( log_list, next );
    ctrl.log_q_gone++;
#ifndef MAS_NO_THREADS
    pthread_rwlock_unlock( &logger_queue_rwlock );
    /* mas_pthread_mutex_unlock( &logger_queue_mutex ); */
#endif
    mas_log_delete_loginfo( li );
  }
  mas_delete_logger_list(  );
  /* HMSG( "CLEARED logger queue : %d [%lu-%lu=%ld]", mas_logger_list( 0 ) ? 1 : 0, ctrl.log_q_came, ctrl.log_q_gone, */
  /*       ctrl.log_q_came - ctrl.log_q_gone );                                                                       */

  /* HMSG( "cleaned logger queue : %d [%lu-%lu=%ld]", mas_logger_list( 0 ) ? 1 : 0, ctrl.log_q_came, ctrl.log_q_gone, */
  /*       ctrl.log_q_came - ctrl.log_q_gone );                                                                       */
}

static int
mas_vlog( const char *func, int line, int merrno, const char *fmt, va_list args )
{
  mas_loginfo_list_head_t *log_list = NULL;
  char buffer[1024 * 8];
  mas_loginfo_t *li = NULL;

  log_list = mas_logger_list( 1 );
  /* pthread_t pth;                           */
  /* th_type_t thtype;                        */
  /* const mas_channel_t *pchannel = NULL;    */
  /* const mas_lcontrol_t *plcontrol = NULL; */
  /* const mas_rcontrol_t *prcontrol = NULL;  */

  /* pthread_mutex_lock( &ctrl.thglob.logger_mutex );   */
  /* pthread_mutex_unlock( &ctrl.thglob.logger_mutex ); */
  if ( log_list )
  {
    vsnprintf( buffer, sizeof( buffer ), fmt, args );

    /* WMSG( "loginfo size : %lu", sizeof( mas_loginfo_t ) ); */

    li = mas_malloc( sizeof( mas_loginfo_t ) );
    memset( li, 0, sizeof( mas_loginfo_t ) );
    li->message = mas_strdup( buffer );
    li->line = line;
#ifdef MS_DUP_FUNC_NAME
    li->func = mas_strdup( func );
#else
    li->func = ( func );
#endif
#ifndef MAS_NO_THREADS
    li->pth = mas_pthread_self(  );
    li->thtype = mas_thself_type(  );
    li->pchannel = mas_thself_pchannel(  );
    li->plcontrol = mas_thself_plcontrol(  );
    li->prcontrol = mas_thself_prcontrol(  );
#endif
    li->pid = getpid(  );
    li->lserial = li->plcontrol ? li->plcontrol->h.serial : 0;
    li->lstatus = li->plcontrol ? li->plcontrol->h.status : 0;
    li->rserial = li->prcontrol ? li->prcontrol->h.serial : 0;
    li->rstatus = li->prcontrol ? li->prcontrol->h.status : 0;
    li->logtime = mas_double_time(  );
    li->lerrno = merrno;
    li->serial = ctrl.log_q_came;
    /* errno = 0; */

#ifndef MAS_NO_THREADS
    /* mas_pthread_mutex_lock( &ctrl.thglob.log_mutex ); */
    /* mas_pthread_mutex_lock( &logger_queue_mutex ); */
    pthread_rwlock_wrlock( &logger_queue_rwlock );
#endif
    MAS_LIST_ADD( log_list, li, next );
    ctrl.log_q_came++;
    ctrl.log_q_mem += strlen( li->message );

#ifndef MAS_NO_THREADS
    pthread_rwlock_unlock( &logger_queue_rwlock );
    /* mas_pthread_mutex_unlock( &logger_queue_mutex ); */
    /* mas_pthread_mutex_unlock( &ctrl.thglob.log_mutex ); */
#endif
  }
  /* HMSG( "logger queue : %d [%lu-%lu=%ld]", mas_logger_list( 0 ) ? 1 : 0, ctrl.log_q_came, ctrl.log_q_gone, */
  /*       ctrl.log_q_came - ctrl.log_q_gone );                                                               */
  return 0;
}

static int
mas_vlog_lim( const char *func, int line, int merrno, const char *fmt, va_list args )
{
  if ( !ctrl.log_offmem && ctrl.log_q_mem > 200000000 )
  {
    ctrl.log_offmem = 1;
    mas_log_unlim( FL, 0, "memory ....." );
  }
  else if ( ctrl.log_offmem && ctrl.log_q_mem < 150000000 )
  {
    ctrl.log_offmem = 0;
    mas_log_unlim( FL, 0, "... memory" );
  }
  if ( !ctrl.log_disabled && !ctrl.log_offmem && !opts.nolog )
    mas_vlog( func, line, merrno, fmt, args );
  return 0;
}

int
mas_log( const char *func, int line, int merrno, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mas_vlog_lim( func, line, merrno, fmt, args );
  va_end( args );
  return r;
}

int
mas_log_unlim( const char *func, int line, int merrno, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mas_vlog( func, line, merrno, fmt, args );
  va_end( args );
  return r;
}