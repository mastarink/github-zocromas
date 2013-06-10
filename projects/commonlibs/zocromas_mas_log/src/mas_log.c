#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


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
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/thtools/mas_thread_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;




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

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno ); */
}

static int
mas_vlog( const char *func, int line, int merrno, const char *fmt, va_list args )
{
  mas_loginfo_list_head_t *log_list = NULL;
  char buffer[1024 * 8];
  mas_loginfo_t *li = NULL;

  if ( !ctrl.log_offmem && ctrl.do_log && !ctrl.log_disabled )
  {
    if ( ctrl.log_stopped )
    {
      EMSG( "LOG STOPPED" );
    }
    else
    {
      log_list = mas_logger_queue( 1 );
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
        li->tid = mas_gettid(  );
        li->lserial = li->plcontrol ? li->plcontrol->h.serial : 0;
        li->lstatus = li->plcontrol ? li->plcontrol->h.status : 0;
        li->rserial = li->prcontrol ? li->prcontrol->h.serial : 0;
        li->rstatus = li->prcontrol ? li->prcontrol->h.status : 0;
        li->logtime = mas_double_time(  );
        li->delta_thread = li->logtime - mas_thself_double_time(  );
        mas_thself_set_double_time( li->logtime );
        li->lerrno = merrno;
        li->serial = ctrl.log_q_came;
        /* NO: errno = 0; */

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
        if ( ctrl.log_q_came - ctrl.log_q_gone > 10 && ctrl.threads.n.logger.thread )
          pthread_cond_broadcast( &ctrl.thglob.logger_wait_cond );
      }
      /* HMSG( "logger queue : %d [%lu-%lu=%ld]", mas_logger_queue( 0 ) ? 1 : 0, ctrl.log_q_came, ctrl.log_q_gone, */
      /*       ctrl.log_q_came - ctrl.log_q_gone );                                                               */
    }
  }
  return 0;
}

static int
mas_log_unlim(  const char *func, int line, int merrno, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mas_vlog( func, line, merrno, fmt, args );
  va_end( args );
  return r;
}

static int
mas_vlog_lim( const char *func, int line, int merrno, const char *fmt, va_list args )
{
  if ( !ctrl.log_offmem && ctrl.log_q_mem > 200000000 )
  {
    ctrl.log_offmem = 1;
    mas_log_unlim(  FL, 0, "memory ....." );
  }
  else if ( ctrl.log_offmem && ctrl.log_q_mem < 150000000 )
  {
    ctrl.log_offmem = 0;
    mas_log_unlim(  FL, 0, "... memory" );
  }
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
