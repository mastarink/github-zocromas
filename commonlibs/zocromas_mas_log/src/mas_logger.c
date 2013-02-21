#include <mastar/wrap/mas_std_def.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#ifndef MAS_NO_THREADS
#  include <pthread.h>
#endif
#include <sys/prctl.h>


#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
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

#include "mas_log.h"
#include "mas_logger.h"

/*
this:
  mas_logger.c
related:
  mas_log.c
  mas_log_types.h
  mas_log.h
  mas_logger.h
more:
  mas_ticker.c
  mas_watcher.c

*/

static mas_loginfo_list_head_t *logger_list;

#ifndef MAS_NO_THREADS
/* mas_th_types.h */

/* pthread_mutex_t logger_queue_mutex = PTHREAD_MUTEX_INITIALIZER; */


/* static pthread_mutex_t logger_write_mutex = PTHREAD_MUTEX_INITIALIZER; */

pthread_rwlock_t logger_queue_rwlock;
pthread_mutex_t logger_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

mas_loginfo_list_head_t *
mas_logger_list( int create )
{
  if ( create && !logger_list )
  {
    logger_list = mas_malloc( sizeof( mas_loginfo_t ) );
    memset( logger_list, 0, sizeof( mas_loginfo_t ) );
    MAS_LIST_INIT( logger_list );
  }
  return logger_list;
}

void
mas_delete_logger_list( void )
{
  mas_free( logger_list );
  logger_list = NULL;
}

static void
mas_logger_write( mas_loginfo_t * li )
{
  if ( li )
  {
    double ltime;
    static double logger_start_time = 0;

    if ( !logger_start_time )
      logger_start_time = mas_double_time(  );

    if ( li->message )
    {
      if ( ctrl.logpath )
      {
        /* mas_pthread_mutex_lock( &logger_write_mutex ); */
        if ( !ctrl.logfile )
        {
          ctrl.logfile = mas_fopen( ctrl.logpath, "a" );
        }
        /* mas_pthread_mutex_unlock( &logger_write_mutex ); */
        if ( ctrl.logfile )
        {
          /* MFP( "%s : %20.5f\n", __func__, logger_start_time ); */
          /* sleep( 1 ); */
          /* mas_nanosleep( 0.25 ); */
          /* mas_nanosleep( 1.25 ); */
        }
        else
        {
#ifdef EMSG_ONCE
          EMSG_ONCE( "log file '%s' not opened", ctrl.logpath );
#endif
        }
        if ( ctrl.logfile )
        {
          static unsigned long serial = 0;
          static double prevlilogtime = 0.;
          double fromlastlog;

          serial++;
          ltime = mas_double_time(  );
          fromlastlog = ( li->logtime - prevlilogtime ) * 1.E6;
          /* if ( opts.logdir )                                                              */
          /* {                                                                               */
          /*   MFP( "%s : (%10.5f) logdir='%s'\n", __func__, ( ltime - logger_start_time ), opts.logdir );     */
          /* }                                                                               */
          /* MFP( "%s : %20.5f (%10.5f) : '%s'\n", __func__, ltime, ( ltime - logger_start_time ), li->message ); */

          /* fprintf( ctrl.logfile, "%16.5f + %7.5f : %16.5f (%7.5f) : %-25s:%03d %s:R%lu:%u @ L%lu:%u: {%s}\n", li->logtime, */
          /* mas_pthread_mutex_lock( &logger_write_mutex ); */
#ifndef MAS_NO_THREADS
          fprintf( ctrl.logfile, "%03lu/%03lu. %18.7f + %12.2f D%7.2f (%8.2f) :%03d:%-25s:%u %s:R%lu:%u @ L%lu:%u: {%s}\n", serial,
                   li->serial, li->logtime, fromlastlog > 1.E14 ? 0 : fromlastlog, ( ltime - li->logtime ) * 1E3,
                   ( li->logtime - ctrl.start_time ) * 1E3, li->line, li->func ? li->func : "-", li->pid,
                   mas_thread_type_name( li->thtype ), li->rserial, li->rstatus, li->lserial, li->lstatus, li->message );
#else
          fprintf( ctrl.logfile, "%03lu/%03lu. %18.7f + %12.2f D%7.2f (%8.2f) :%03d:%-25s:%u *:R%lu:%u @ L%lu:%u: {%s}\n", serial,
                   li->serial, li->logtime, fromlastlog > 1.E14 ? 0 : fromlastlog, ( ltime - li->logtime ) * 1E3,
                   ( li->logtime - ctrl.start_time ) * 1E3, li->line, li->func ? li->func : "-", li->pid,
                   li->rserial, li->rstatus, li->lserial, li->lstatus, li->message );
#endif
          /* mas_pthread_mutex_unlock( &logger_write_mutex ); */
          if ( li->lerrno )
          {
            char *errbuf;
            char *se = NULL;

            errbuf = mas_malloc( 4096 );
            se = strerror_r( li->lerrno, errbuf, sizeof( errbuf ) );
            /* fprintf( ctrl.logfile,  "ERROR %u : %s\n", li->lerrno, se ? se : NULL); */
            /* mas_pthread_mutex_lock( &logger_write_mutex ); */
#ifndef MAS_NO_THREADS
            fprintf( ctrl.logfile, "%18.7f + %18.7f D%9.7f (%10.7f) :%-25s:%03d: %s:R%lu:%u @ L%lu:%u: ERROR{%d:%s}\n",
                     li->logtime, fromlastlog > 1.E14 ? 0 : fromlastlog, ltime - li->logtime, ( li->logtime - ctrl.start_time ),
                     li->func ? li->func : "-", li->line, mas_thread_type_name( li->thtype ), li->rserial, li->rstatus,
                     li->lserial, li->lstatus, li->lerrno, se );
#else
            fprintf( ctrl.logfile, "%18.7f + %18.7f D%9.7f (%10.7f) :%-25s:%03d: *:R%lu:%u @ L%lu:%u: ERROR{%d:%s}\n",
                     li->logtime, fromlastlog > 1.E14 ? 0 : fromlastlog, ltime - li->logtime, ( li->logtime - ctrl.start_time ),
                     li->func ? li->func : "-", li->line, li->rserial, li->rstatus, li->lserial, li->lstatus, li->lerrno, se );
#endif
            /* mas_pthread_mutex_unlock( &logger_write_mutex ); */
            mas_free( errbuf );
          }

          prevlilogtime = li->logtime;
          /* mas_pthread_mutex_lock( &logger_write_mutex ); */
          fflush( ctrl.logfile );
          /* mas_pthread_mutex_unlock( &logger_write_mutex ); */
        }
      }
      else
      {
        static unsigned errdone = 0;

        if ( !errdone )
        {
#ifdef EMSG
          EMSG( "logpath not set" );
#endif
          errdone = 1;
        }
      }
    }
    mas_log_delete_loginfo( li );
  }
}

void
mas_logger_cleanup( void *arg )
{
  ctrl.keep_logging = 0;
#ifdef FMSG
  FMSG( "TO FLUSH LOGGER" );
#endif
  ctrl.log_disabled = 1;
  mas_logger_flush(  );
  mas_log_clean_queue(  );
  mas_logger_close(  );
  MAS_LOG( "logger cleanup" );
#ifdef FMSG
  FMSG( "LOGGER CLEANUP DONE" );
#endif
}

#ifndef MAS_NO_THREADS
static void *
mas_logger_th( void *arg )
{
  int lf = -1;

  ctrl.logger_tid = mas_gettid(  );
  if ( prctl( PR_SET_NAME, ( unsigned long ) "zoclog" ) < 0 )
  {
    P_ERR;
  }

  MAS_LOG( "logger start" );
  mas_in_thread( MAS_THREAD_LOGGER, NULL, NULL );

  /* FMSG( "(!) logger %d\x1b[K", ctrl.keep_logging ); */
  ctrl.keep_logging = 1;
  MAS_LOG( "logger start [%lx]", ctrl.logger_thread );
  pthread_cleanup_push( mas_logger_cleanup, NULL );
  while ( ( lf = mas_logger_flush(  ) ) == 0 || ctrl.keep_logging )
  {
    mas_nanosleep( 0.1 );
  }
  pthread_cleanup_pop( 1 );
  MAS_LOG( "logger stop" );
#  ifdef FMSG
  FMSG( "LOGGER STOP" );
#  endif
  mas_pthread_exit( NULL );
  return NULL;
}

static size_t logger_stacksize = 0;
static void *logger_stackaddr = NULL;
int
mas_logger_start( void )
{
  int r = 0;

  if ( !ctrl.logger_thread )
  {
    pthread_setconcurrency( 4 );
    MAS_LOG( "starting logger th. [concurrency:%u]", pthread_getconcurrency(  ) );
    {
      int r;

      r = pthread_attr_getstack( &ctrl.thglob.logger_attr, &logger_stackaddr, &logger_stacksize );
      /* thMSG( "creating logger thread stack:%lu @ %p", logger_stacksize, logger_stackaddr ); */
    }
    /* r = mas_xpthread_create( &ctrl.logger_thread, mas_logger_th, MAS_THREAD_LOGGER, NULL ); */
    r = pthread_create( &ctrl.logger_thread, &ctrl.thglob.logger_attr, mas_logger_th, NULL );
    /* thMSG( "(%d) created(?) logger thread [%lx]", r, ctrl.logger_thread ); */
    MAS_LOG( "(%d) created(?) logger thread [%lx]", r, ctrl.logger_thread );
    if ( opts.logdir && ctrl.logpath )
    {
      ctrl.keep_logging = 1;
    }
  }
  else
  {
    MAS_LOG( "running w/o logger th." );
  }
  return r;
}

int
mas_logger_stop( void )
{
  int r = 0;

  if ( ctrl.logger_thread )
  {
    MAS_LOG( "cancelling logger" );
    /* mMSG( "cancelling logger [%lx]", ctrl.logger_thread ); */
    /* mas_pthread_mutex_lock( &logger_write_mutex ); */
    mas_pthread_cancel( ctrl.logger_thread );
    /* mas_pthread_mutex_unlock( &logger_write_mutex ); */
    /* mMSG( "stopping (cancelling) logger" ); */
    /* MAS_LOG( "canceled logger" ); */
    /* MAS_LOG( "joining logger" ); */
    /* mMSG( "joining logger" ); */
    mas_xpthread_join( ctrl.logger_thread );
    /* mMSG( "stoped logger" ); */
    /* MAS_LOG( "stopped logger" ); */
    ctrl.logger_thread = ( pthread_t ) 0;
  }
  else
  {
    MAS_LOG( "logger already cancelled" );
  }
  return r;
}
#endif

int
mas_logger_flush( void )
{
  int r = -1;

  while ( logger_list && !MAS_LIST_EMPTY( logger_list ) )
  {
    mas_loginfo_t *li;

#ifndef MAS_NO_THREADS
    /* pthread_mutex_unlock( &ctrl.thglob.logger_mutex ); */

    pthread_rwlock_wrlock( &logger_queue_rwlock );
#endif
    li = MAS_LIST_FIRST( logger_list );
    /* MAS_LIST_REMOVE( logger_list, li, mas_loginfo_s, next ); */
    MAS_LIST_REMOVE_HEAD( logger_list, next );
    ctrl.log_q_gone++;
    ctrl.log_q_mem -= strlen( li->message );
#ifndef MAS_NO_THREADS
    pthread_rwlock_unlock( &logger_queue_rwlock );
#endif
    mas_logger_write( li );

    /* if ( ctrl.log_disabled )                                                                                         */
    /* {                                                                                                                */
    /*   FMSG( "LOGGER FLUSH [%lu-%lu=%ld]...", ctrl.log_q_came, ctrl.log_q_gone, ctrl.log_q_came - ctrl.log_q_gone ); */
    /* }                                                                                                                */

    r = 0;
  }
  if ( ctrl.log_disabled )
  {
#ifdef FMSG
    FMSG( "LOGGER FLUSH [%lu-%lu=%ld]...", ctrl.log_q_came, ctrl.log_q_gone, ctrl.log_q_came - ctrl.log_q_gone );
#endif
  }
  return r;
}

int
mas_logger_close( void )
{
  int r = 0;

  if ( ctrl.logfile )
  {
    FILE *f;

    /* mas_pthread_mutex_lock( &logger_write_mutex ); */
    f = ctrl.logfile;
    /* mas_pthread_mutex_unlock( &logger_write_mutex ); */
    ctrl.logfile = NULL;
    mas_fclose( f );
  }
  return r;
}
