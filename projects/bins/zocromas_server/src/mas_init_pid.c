#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/tools/mas_tools.h>


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifndef MAS_SERVER_NOLOG
#  include <mastar/log/mas_logger.h>
#  include <mastar/log/mas_log.h>
#endif

#include <mastar/types/mas_control_types.h>

#include "mas_init_pid.h"

static int
cref( const char *pidpath, const char *dir )
{
  EVAL_PREPARE;
  int r = 0;

  /* mas_clear_last_error(  ); */
  YEVALM( r, mas_open( pidpath, O_CREAT | O_RDWR | O_EXCL, S_IWUSR | S_IRUSR ), "(%d) file:%s", pidpath );
  if ( r < 0 )
  {
    const mas_std_error_t *e;

    /* char errbuf[1024]; */
    /* char *se = NULL; */

    e = mas_get_last_error(  );
    if ( e )
    {
      switch ( e->merrno )
      {
      case ENOENT:
        r = 0;
        HMSG( "(%d)CREF - NO DIR : %s", r, pidpath );
        IEVAL( r, mkdir( dir, S_IWUSR | S_IRUSR | S_IXUSR ) );
        if ( r >= 0 )
          r = -999;
        break;
      }
      /* se = strerror_r( e->merrno, errbuf, sizeof( errbuf ) ); */
      /* HMSG( "(%d)CREF 1 : %s [errno:%d] %s", r, pidpath, e->merrno, se ); */
    }
  }
  return r;
}

static int
_mas_init_pid( mas_options_t * popts, const char *shash_name )
{
  EVAL_PREPARE;
  int r = 0;

  char *pidpath = NULL;

  r = 0;
  pidpath = mas_strdup( popts->dir.pids );
  pidpath = mas_strcat_x( pidpath, shash_name );

  r = cref( pidpath, popts->dir.pids );
  if ( r == -999 )
    r = cref( pidpath, popts->dir.pids );
  HMSG( "(%d)PIDPATH 1 : %s", r, pidpath );
  if ( r < 0 )
  {
    r = 0;
    IEVAL( r, mas_open( pidpath, O_RDWR | O_EXCL, S_IWUSR | S_IRUSR ) );
  }
  if ( r >= 0 )
  {
    CTRL_PREPARE;

    ctrl.pidfd = r;
    {
      int er = 0;

      IEVALE( r, lockf( ctrl.pidfd, F_TLOCK, 0 ), &er );
      HMSG( "(%d:%d)PIDLCK : %s - %s before", r, errno, pidpath, r < 0 && ( er == EACCES || er == EAGAIN ) ? "LOCKED" : "UNLOCKED" );
    }
    if ( r >= 0 )
    {
      ssize_t lb;
      char buf[64];

      lb = snprintf( buf, sizeof( buf ), "%u", getpid(  ) );
      IEVAL( r, ftruncate( ctrl.pidfd, 0 ) );
      IEVAL( r, write( ctrl.pidfd, buf, lb ) );
      HMSG( "PIDWRT: %d pid:%u W:%ld", r, ctrl.threads.n.main.pid, lb );
      ctrl.pidfilesv.c = mas_add_argv_arg_nodup( ctrl.pidfilesv.c, &ctrl.pidfilesv.v, pidpath );
      pidpath = NULL;
    }
    else
    {
      if ( ctrl.pidfd >= 0 )
        mas_close( ctrl.pidfd );
      ctrl.pidfd = r;
    }
  }
  if ( pidpath )
    mas_free( pidpath );
  HMSG( "(%d)INIT PID", r );
  return r;
}

int
mas_pids_init( mas_options_t * popts, const char **message )
{
  int r = 0;

  /* if ( !popts->wopidfile ) */
  if ( !OPT_QFLAG( popts, wopidfile ) )
  {
    size_t shash_namebuf_size = 512;
    char *shash_namebuf = NULL;

    /* ctrl.threads.n.main.tid = mas_gettid(  );          */
    /* ctrl.threads.n.main.pid = getpid(  );              */
    /* ctrl.threads.n.main.thread = mas_pthread_self(  ); */
    /* ctrl.pserver_thread = &ctrl.threads.n.main;        */

    shash_namebuf = mas_malloc( shash_namebuf_size );
    MAS_LOG( "(%d) init pids", r );
    if ( shash_namebuf )
    {
      CTRL_PREPARE;

      *shash_namebuf = 0;
      WMSG( "PIDSDIR: %s", popts->dir.pids );
      /* if ( popts->single_instance && popts->dir.pids ) */
      if ( OPT_QFLAG( popts, single_instance ) && popts->dir.pids )
      {
        snprintf( shash_namebuf, shash_namebuf_size, "/zocromas_%s.pid", ctrl.is_client ? "client" : "server" );
      }
      /* else if ( popts->single_child && popts->dir.pids ) */
      else if ( OPT_QFLAG( popts, single_child ) && popts->dir.pids )
      {
        snprintf( shash_namebuf, shash_namebuf_size, "/zocromas_%s.%u.pid", ctrl.is_client ? "client" : "server", getppid(  ) );
      }
      if ( *shash_namebuf )
      {
        r = _mas_init_pid( popts, shash_namebuf );
      }
      HMSG( "(%d) INIT PID done", r );
      mas_free( shash_namebuf );
    }
    MAS_LOG( "(%d) init pids done", r );

    HMSG( "(%d) INIT PIDs done", r );
  }
  if ( message )
    *message = __func__;
  return r < 0 ? r : 0;
}

void
mas_pids_destroy( mas_options_t * popts )
{
  CTRL_PREPARE;
  {
    if ( ctrl.pidfd > 0 )
    {
      /* char *pidpath;                           */
      /*                                          */
      /* pidpath = mas_strdup( popts-> dir.pids );    */
      /* pidpath = mas_strcat_x( pidpath, name ); */

      mas_close( ctrl.pidfd );
      /* unlink( pidpath ); */
    }
    ctrl.pidfd = 0;
  }
  if ( ctrl.threads.n.daemon.pid && ctrl.threads.n.daemon.pid == getpid(  ) )
    for ( int ifil = 0; ifil < ctrl.pidfilesv.c; ifil++ )
    {
      /* HMSG( "PID FILE %d. %s", ifil, ctrl.pidfilesv.v[ifil] ); */
      unlink( ctrl.pidfilesv.v[ifil] );
    }

  mas_del_argv( ctrl.pidfilesv.c, ctrl.pidfilesv.v, 0 );
  ctrl.pidfilesv.c = 0;
  ctrl.pidfilesv.v = NULL;
}

__attribute__ ( ( constructor( 1001 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_constructor( IL, 0 );
}
