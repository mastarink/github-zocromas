#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>

#include <mastar/log/mas_log.h>



#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/thtools/mas_thread_tools.h>

#include "mas_msg_def.h"
#ifdef MAS_USE_CURSES
#  include <curses.h>
#  include "mas_curses.h"
#endif

#include "mas_msg_tools.h"

#define MAS_DROP_PREFIX "?zocromas_"

/*
this:
  mas_msg_tools.c
related:
  mas_thread_tools.c
  mas_tools_def.h
  mas_common_types.h
more:
  mas_control.c
  mas_opts.c
  mas_log.c

*/

int
mas_msg_set_file( const char *path )
{
  int r = 0;

  MAS_LOG( "set.msg" );
  if ( ctrl.msgfile )
  {
    MAS_LOG( "set.msg" );
    if ( ctrl.msgfile != ctrl.stderrfile && EOF == fclose( ctrl.msgfile ) )
    {
      IEVAL( r, -1 );
    }
    MAS_LOG( "set.msg" );
    ctrl.msgfile = NULL;
  }
  r = 0;
  MAS_LOG( "set.msg" );
  if ( path )
  {
    MAS_LOG( "set.msg" );
    ctrl.msgfile = fopen( path, "a" );
    /* ffcntl( ctrl.msgfile, F_SETFL, O_SYNC ); */
    MAS_LOG( "set.msg" );
    if ( ctrl.msgfile )
    {
      /* for /dev/pts/... : ERROR{29:Illegal seek} */
      errno = 0;
      MAS_LOG( "set.msg" );
      IEVAL( r, setvbuf( ctrl.msgfile, NULL, _IONBF, 0 ) );
      MAS_LOG( "set.msg" );
    }
    else
    {
      IEVAL( r, -1 );
    }
    MAS_LOG( "set.msg" );
  }
  MAS_LOG( "set.msg" );
  return r;
}

static int
__mas_msg_prefix( int fdetails, pid_t pid, const char *prefix_fmt, const char *prefix )
{
  const char *qprefix;

  qprefix = prefix;
  {
    int lqp = strlen( MAS_DROP_PREFIX );

    if ( qprefix && *qprefix && 0 == strncmp( prefix, MAS_DROP_PREFIX, lqp ) )
      qprefix = qprefix + lqp;
  }

#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    if ( fdetails )
    {
      if ( ctrl.is_server && prefix && *prefix )
      {
        /* MFP( prefix_fmt ? prefix_fmt : " %-6s ", prefix ); */
      }
      if ( pid == ctrl.main.pid )
      {
        wattron( w_win, A_BOLD );
        MFP( "+" );
        wattroff( w_win, A_BOLD );
        /* wcolor_set( w_win, 2, NULL ); */
        /* MFP( ":%5u", pid );           */
        wcolor_set( w_win, 1, NULL );
        MFP( ":%1c:", ctrl.keep_listening ? 'S' : '-' );
      }
      else
      {
        MFP( ">%5u:%1d:", pid, ctrl.keep_listening );
      }
    }
  }
  else
#endif
  {
    if ( fdetails )
    {
      if ( ctrl.is_server && qprefix && *qprefix )
      {
        MFP( "\x1b[1;43;33m" );
        MFP( prefix_fmt ? prefix_fmt : "-- %-5s", qprefix );
        MFP( "\x1b[0m" );
      }
      if ( pid == ctrl.threads.n.main.pid )
      {
        MFP( "\x1b[1;31mMain\x1b[0m:%5u.%u", pid, ctrl.restart_cnt );
        if ( ctrl.is_server )
          MFP( " :%1d: ", ctrl.keep_listening );
      }
      else if ( pid == ctrl.threads.n.child.pid )
      {
        MFP( "\x1b[1;31mChild\x1b[0m:%5u.%u", pid, ctrl.restart_cnt );
        if ( ctrl.is_server )
          MFP( " :%1d: ", ctrl.keep_listening );
      }
      else
      {
        MFP( "\x1b[1;33mfork\x1b[0m:%5u", pid );
        if ( ctrl.is_server )
          MFP( " :%1d: ", ctrl.keep_listening );
      }
    }
    else
    {
      MFP( "\x1b[1;43;33m" );
      MFP( prefix_fmt ? prefix_fmt : " %s ", qprefix );
    }
  }
  return 0;
}

static int
__mas_msg_thread_info( int fdetails, th_type_t thtype )
{
  if ( fdetails )
  {
    pthread_t pth = 0;

    pth = pthread_self(  );
    mas_lcontrol_t *plcontrol = NULL;
    mas_rcontrol_t *prcontrol = NULL;

    plcontrol = mas_thself_plcontrol(  );
    prcontrol = mas_thself_prcontrol(  );
#ifdef MAS_USE_CURSES
    if ( use_curses )
    {
      unsigned color = 0;

      wattron( w_win, A_BOLD );
      /* MFP( "pth:" ); */
      wcolor_set( w_win, color, NULL );
      switch ( thtype )
      {
      case MAS_THREAD_MASTER:
        MFP( "M0:%u ", ctrl.status );
        break;
      case MAS_THREAD_LISTENER:
        MFP( "L%lu:%u ", plcontrol ? plcontrol->h.serial : 0xffffffff, plcontrol ? plcontrol->h.status : 999 );
        break;
      case MAS_THREAD_TRANSACTION:
        MFP( "R%lu:%u @ L%lu:%u ", prcontrol ? prcontrol->h.serial : 0xffffffff, prcontrol ? prcontrol->h.status : 999,
             prcontrol->plcontrol ? prcontrol->plcontrol->h.serial : 0xffffffff,
             prcontrol->plcontrol ? prcontrol->plcontrol->h.status : 999 );
        break;
      case MAS_THREAD_LOGGER:
      case MAS_THREAD_WATCHER:
      case MAS_THREAD_TICKER:
        MFP( "%s[%lx]", mas_thread_type_name( thtype ), pth );
        break;
      default:
        MFP( "[%lx]", pth );
        break;
      }
      wcolor_set( w_win, 1, NULL );
      MFP( ":" );
      wattroff( w_win, A_BOLD );
    }
    else
#endif
    {
      switch ( thtype )
      {
      case MAS_THREAD_MASTER:
        MFP( "pth:\x1b[1;%dm(%s) M0:%u\x1b[0m:", 33, mas_thread_type_name( thtype ), ctrl.status );
        break;
      case MAS_THREAD_LISTENER:
        MFP( "pth:\x1b[1;%dm(%s) L%lu:%u\x1b[0m:", 34, mas_thread_type_name( thtype ), plcontrol ? plcontrol->h.serial : 0xffffffff,
             plcontrol ? plcontrol->h.status : 999 );
        break;
      case MAS_THREAD_TRANSACTION:
        MFP( "pth:\x1b[1;%dm(%s) R%lu:%u @ L%lu:%u\x1b[0m:", 35, mas_thread_type_name( thtype ),
             prcontrol ? prcontrol->h.serial : 0xffffffff, prcontrol ? prcontrol->h.status : 999,
             prcontrol->plcontrol ? prcontrol->plcontrol->h.serial : 0xffffffff,
             prcontrol->plcontrol ? prcontrol->plcontrol->h.status : 999 );
        break;
      default:
        MFP( "pth:\x1b[1;%dm(%s) [%lx]\x1b[0m:", 38, mas_thread_type_name( thtype ), pth );
        break;
      }
    }
  }
  return 0;
}

static int
__mas_msg_code_position( int fdetails, int line, const char *func )
{
  if ( fdetails )
  {
    if ( opts.f.bit.msg_funline )
    {
      MFP( " L%03d:%-25s:", line, func );
      /* MFP( "\t" ); */
    }
    else
    {
      MFP( "<>" );
    }
  }
  return 0;
}

int
__mas_msg_message( int fdetails, const char *message )
{
#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    MFP( fdetails ? "%-15s" : "%-50s", message );
  }
  else
#endif
  {
    MFP( fdetails ? "\x1b[K%-15s" : "\x1b[0m \x1b[K%-50s\x1b[0m", message );
  }
  return 0;
}

int
__mas_msg_suffix( int fdetails, const char *suffix )
{
#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    MFP( "   %s", suffix ? suffix : "" );
  }
  else
#endif
  {
    MFP( "   \x1b[1;43;33m%s\x1b[0m\n", suffix ? suffix : "" );
  }
  return 0;
}

int
__mas_msg_suffix2( int fdetails, const char *suffix )
{
  return __mas_msg_suffix( 0, suffix );
}

int
__mas_msg_consume( int fdetails )
{
  extern unsigned long memory_balance;
  unsigned long elapsed_time;
  unsigned long cur_time = ( unsigned long ) time( NULL );

  elapsed_time = cur_time - ctrl.stamp.start_time;
  if ( !fdetails )
  {
    MFP( "{e%03d} +%05lu ", errno, elapsed_time );
  }
  else
  {
#ifdef MAS_USE_CURSES
    if ( use_curses )
    {
      MFP( "{e%03d} %-5lus:%-5lu:", errno, elapsed_time, memory_balance );
    }
    else
#endif
    {
      MFP( "  " );
      MFP( "{e%03d} (%3lus:%5lu) ", errno, elapsed_time, memory_balance );
    }
  }
  return 0;
}

static int
__mas_vmsg( const char *func, int line, int allow, int is_trace, int details, int msgcolor, const char *prefix_fmt,
            const char *prefix, const char *suffix, const char *fmt, va_list args, th_type_t thtype )
{
  int r = 0;
  char message[4096];
  pid_t pid;

  pid = getpid(  );
  if ( !ctrl.stamp.start_time )
    ctrl.stamp.start_time = ( unsigned long ) time( NULL );
  vsnprintf( ( char * ) message, sizeof( message ), fmt, args );
#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    MFP( "\n*" );
  }
  else
#endif
  {
    MFP( "\r" );
  }
  if ( *message == *MAS_SEPARATION_LINE )
  {
#ifdef MAS_USE_CURSES
    if ( use_curses )
    {
      MFP( "%s", message );
    }
    else
#endif
    {
      MFP( "{e%03d} @\x1b[K\x1b[0;43;30m%s\x1b[0m\n", errno, message );
    }
  }
  else
  {
    __mas_msg_consume( details );
    __mas_msg_prefix( details, pid, prefix_fmt, prefix );
    if ( ctrl.is_server )
      __mas_msg_thread_info( details, thtype );
    __mas_msg_code_position( details, line, func );
    __mas_msg_message( details, message );
    __mas_msg_suffix( details, suffix );
  }
  return r;
}

int
mas_msg( const char *func, int line, int allow, int is_trace, int details, int msgcolor,
         const char *prefix_fmt, const char *prefix, const char *suffix, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  /* mas_channel_t *pchannel = NULL; */
  va_start( args, fmt );
  /* if (is_trace && ){} */
  if ( allow )
  {
    th_type_t thtype;

    thtype = mas_thself_type(  );
    {
      int can = 0;

      can = ctrl.messages;
      if ( can )
      {
        switch ( thtype )
        {
        case MAS_THREAD_MASTER:
          can = opts.f.bit.msg_trace_main;
          break;
        case MAS_THREAD_LISTENER:
          can = opts.f.bit.msg_trace_listener;
          break;
        case MAS_THREAD_TRANSACTION:
          can = opts.f.bit.msg_trace_transaction;
          break;
        default:
          can = 1;
          break;
        }
      }
      if ( allow == 777 )
        can = 1;
      /* if ( !is_trace                                                               */
      /*      || ( thtype == MAS_THREAD_MASTER && opts.f.bit.msg_trace_main )               */
      /*      || ( thtype == MAS_THREAD_LISTENER && opts.f.bit.msg_trace_listener )         */
      /*      || ( thtype == MAS_THREAD_TRANSACTION && opts.f.bit.msg_trace_transaction ) ) */
      if ( can )
      {
        pthread_mutex_lock( &ctrl.thglob.msg_mutex );
        r = __mas_vmsg( func, line, allow, is_trace, details, msgcolor, prefix_fmt, prefix, suffix, fmt, args, thtype );
        pthread_mutex_unlock( &ctrl.thglob.msg_mutex );
      }
    }
  }
  va_end( args );
  return r;
}

int
mas_verror( const char *func, int line, int merrno, const char *fmt, va_list args )
{
  int r = 0;
  th_type_t thtype;

#ifdef MAS_USE_CURSES
  WINDOW *w_win;

  w_win = w_other;
#endif
  thtype = mas_thself_type(  );
  pthread_mutex_lock( &ctrl.thglob.emsg_mutex );
  if ( errno )
  {
    char pref[512];
    char errbuf[1024];
    char *se = NULL;

    se = strerror_r( errno, errbuf, sizeof( errbuf ) );
    snprintf( pref, sizeof( pref ), "(%u:%s) i/s:%u:i/c:%u", errno, se ? se : NULL, ctrl.keep_listening, ctrl.in_client );
    r = __mas_vmsg( func, line, 1, 1, 1, 31, "{  %4s   }", pref, NULL, fmt, args, thtype );
  }
  else
  {
    r = __mas_vmsg( func, line, 1, 1, 1, 31, "{  %4s   }", "E-R-R-O-R", NULL, fmt, args, thtype );
  }
  pthread_mutex_unlock( &ctrl.thglob.emsg_mutex );
  switch ( errno )
  {
  case EINTR:
  case ECONNRESET:
  case ECONNREFUSED:
    /* off : 20121223 *//* ctrl.keep_listening = 0; */
    /* off : 20130102 *//* ctrl.in_client = 0; */
    /* OR ???? : ctrl.fatal = 1; */
    break;
  }
  return r;
}

int
mas_error( const char *func, int line, int merrno, const char *fmt, ... )
{
  int r = 0;
  va_list args;

  va_start( args, fmt );
  r = mas_verror( func, line, merrno, fmt, args );
  va_end( args );
  return r;
}

void
mas_perr( const char *func, int line )
{
  if ( errno )
  {
    static int errcnt = 0;
    char errbuf[1024];
    char *se;

    /* opts.f.bit.msg_tr = 1; */
    errcnt++;
    se = strerror_r( errno, errbuf, sizeof( errbuf ) );
    EMSGfl( func, line, "[error %d] %s (i/c:%d; i/s:%d; fatal:%d)", errno, se, ctrl.in_client, ctrl.keep_listening, ctrl.fatal );
    if ( errcnt < 10 )
    {
    }
    else if ( errcnt < 20 )
      sleep( 1 );
    else
    {
      sleep( 5 );
      errcnt = 0;
    }
    /* sleep(10); */
  }
}

void *
mas_fatal( void )
{
  MFPZ( "pointer error" );
  exit( 11 );
}
