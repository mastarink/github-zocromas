#include <mastar/wrap/mas_std_def.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>

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
mas_log_clean_queue( void )
{
  mas_loginfo_t *li = NULL, *li_tmp;

  ctrl.keep_logging = 0;
  if ( ctrl.log_list )
  {
    /* TODO mutex */
    if ( !MAS_LIST_EMPTY( ctrl.log_list ) )
    {
      MAS_LIST_FOREACH_SAFE( li, ctrl.log_list, next, li_tmp )
      {
        /* mas_pthread_mutex_lock( &logger_queue_mutex ); */
        pthread_rwlock_wrlock( &logger_queue_rwlock );

        MAS_LIST_REMOVE( ctrl.log_list, li, mas_loginfo_s, next );
        ctrl.log_q_gone++;
        pthread_rwlock_unlock( &logger_queue_rwlock );
        /* mas_pthread_mutex_unlock( &logger_queue_mutex ); */

        mas_free( li->message );
        li->message = NULL;
        mas_free( li->func );
        mas_free( li );
      }
    }
    mas_free( ctrl.log_list );
    ctrl.log_list = NULL;
  }
  /* mMSG( "cleaned logger queue" ); */
}

static int
mas_vlog( const char *func, int line, int merrno, const char *fmt, va_list args )
{
  char buffer[1024 * 8];
  mas_loginfo_t *li = NULL;

  /* pthread_t pth;                           */
  /* th_type_t thtype;                        */
  /* const mas_channel_t *pchannel = NULL;    */
  /* const mas_lcontrol_t *plcontrol = NULL; */
  /* const mas_rcontrol_t *prcontrol = NULL;  */

  /* pthread_mutex_lock( &ctrl.thglob.logger_mutex );   */
  /* pthread_mutex_unlock( &ctrl.thglob.logger_mutex ); */

  vsnprintf( buffer, sizeof( buffer ), fmt, args );

  /* FMSG( "loginfo size : %lu", sizeof( mas_loginfo_t ) ); */

  li = mas_malloc( sizeof( mas_loginfo_t ) );
  memset( li, 0, sizeof( mas_loginfo_t ) );
  li->func = mas_strdup( func );
  li->line = line;
  li->message = mas_strdup( buffer );

  li->pth = mas_pthread_self(  );
  li->thtype = mas_thself_type(  );
  li->pchannel = mas_thself_pchannel(  );
  li->plcontrol = mas_thself_plcontrol(  );
  li->prcontrol = mas_thself_prcontrol(  );
  li->lserial = li->plcontrol ? li->plcontrol->h.serial : 0;
  li->lstatus = li->plcontrol ? li->plcontrol->h.status : 0;
  li->rserial = li->prcontrol ? li->prcontrol->h.serial : 0;
  li->rstatus = li->prcontrol ? li->prcontrol->h.status : 0;
  li->logtime = mas_double_time(  );
  li->lerrno = merrno;
  li->serial = ctrl.log_q_came;
  /* errno = 0; */

  /* mas_pthread_mutex_lock( &ctrl.thglob.log_mutex ); */
  /* mas_pthread_mutex_lock( &logger_queue_mutex ); */
  pthread_rwlock_wrlock( &logger_queue_rwlock );
  if ( !ctrl.log_list )
  {
    ctrl.log_list = mas_malloc( sizeof( mas_loginfo_t ) );
    memset( ctrl.log_list, 0, sizeof( mas_loginfo_t ) );
    MAS_LIST_INIT( ctrl.log_list );
  }
  MAS_LIST_ADD( ctrl.log_list, li, next );
  ctrl.log_q_came++;
  ctrl.log_q_mem += strlen( li->message );

  pthread_rwlock_unlock( &logger_queue_rwlock );
  /* mas_pthread_mutex_unlock( &logger_queue_mutex ); */
  /* mas_pthread_mutex_unlock( &ctrl.thglob.log_mutex ); */
  return 0;
}

static int
mas_vlog_lim( const char *func, int line, int merrno, const char *fmt, va_list args )
{
  if ( !ctrl.log_offmem && ctrl.log_q_mem > 200000000 )
  {
    ctrl.log_offmem = 1;
    mas_log_unlim(FL, 0, "memory .....");
  }
  else if ( ctrl.log_offmem && ctrl.log_q_mem < 150000000 )
  {
    ctrl.log_offmem = 0;
    mas_log_unlim(FL, 0, "... memory");
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
