#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef HAVE_LIBUUID
#  include <uuid/uuid.h>
#endif

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>
#include <mastar/log/mas_logger.h>




#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>


#include <mastar/control/mas_control.h>

#ifdef MAS_USE_CURSES
#  include <mastar/msg/mas_curses.h>
#endif

#include "mas_opts.h"
#include "mas_opts_save.h"
#include "mas_opts_restore.h"

#include "mas_cli_options.h"

#include "mas_sig.h"
#include "mas_init.h"


#ifdef MAS_USE_CURSES
int use_curses = 0;
WINDOW *w_rootwin = NULL, *w_main0 = NULL, *w_main = NULL, *w_listen0 = NULL, *w_listen = NULL, *w_trans0 = NULL, *w_trans =
      NULL, *w_other0 = NULL, *w_other = NULL;
#endif

/*
this:
  mas_init.c
related:
  mas_init_server.c
  mas_init_client.c
more:
  mas_init_threads.c
  mas_log.c

  mas_opts.c

*/

static int
mas_init_argv( mas_options_t * popts, int argc, char **argv, char **env )
{
  extern mas_control_t ctrl;

  ctrl.launchervv.v = argv;
  ctrl.launchervv.c = argc;
  ctrl.launcherev.v = env;
  /* if ( argc > 1 ) */
  {
    for ( int ia = 0; ia < argc; ia++ )
    {
      popts->argvv.c = mas_add_argv_arg( popts->argvv.c, &popts->argvv.v, argv[ia] );
    }
    /* for ( int ia = 0; ia < argc; ia++ )                              */
    /* {                                                                */
    /*   mMSG( "@: %d of %d. arg:'%s'", ia, popts-> argvv.c, popts-> argvv.v[ia] ); */
    /* }                                                                */
  }
  HMSG( "INIT ARGV %d", popts->argvv.c );
  return popts->argvv.c;
}

int
mas_init_env( mas_options_t * popts )
{
  char *seopts = NULL;

  if ( popts->env_optsname && ( seopts = getenv( popts->env_optsname ) ) )
  {
    mMSG( "E: %s='%s'", popts->env_optsname, seopts );
    /* popts-> argvv.c = mas_make_argv( seopts, popts-> argvv.c, &popts-> argvv.v, 0 ); */
    popts->argvv.c = mas_add_argv_args( popts->argvv.c, &popts->argvv.v, seopts, 0 );

    /* for ( int ia = 0; ia < popts-> argvv.c; ia++ )                         */
    /* {                                                                */
    /*   mMSG( "E: %d of %d. arg:'%s'", ia, popts-> argvv.c, popts-> argvv.v[ia] ); */
    /* }                                                                */
  }
  /* popts-> argvv.c = mas_make_argv( "-P5001 -H192.168.71.2 -H192.168.71.2:5002 -H192.168.71.2:5003", popts-> argvv.c, &popts-> argvv.v, 0 ); */
  return 0;
}

static int
error_handler_at_init( const char *func, int line, int issys, int rcode, int ierrno, int *perrno, int *pserrno, const char *fmt,
                       const char *msg )
{
  if ( !fmt )
    fmt = " r #%d [%s]";

  /* HMSG( "ERROR HANDLER >>>>>>>>>>>> %d:%s ; r:%d e:%d; msg : %s", line, func, rcode, ierrno, msg ? msg : "-" ); */
  /* mas_set_error( func, line, ierrno, rcode, msg ? msg : "-" ); */
  mas_error( func, line, ierrno, perrno, pserrno, fmt, rcode, msg ? msg : "-" );
  mas_log( func, line, ierrno, fmt, rcode, msg ? msg : "-" );
/********************************************************************************
 *         va_start( args, rcode );                                             *
 *         while ( masierrno = va_arg( args, int ) )                            *
 *         {                                                                    *
 *         const char *fmt;                                                     *
 *         }                                                                    *
 *         va_end( args );                                                      *
 ********************************************************************************/
  if ( perrno )
    *perrno = 0;
  return rcode;
}





static int
mas_pre_init_runpath( char *runpath )
{
  extern mas_control_t ctrl;
  int r = 0;
  const char *pn;

  ctrl.status = MAS_STATUS_START;
  ctrl.error_handler = error_handler_at_init;

  ctrl.start_time = mas_double_time(  );
  ctrl.stamp.start_time = ( unsigned long ) time( NULL );
  /* ctrl.stamp.lts = ( unsigned long ) time( NULL ); */
  ctrl.stamp.first_lts = 0;
  ctrl.status = MAS_STATUS_INIT;


  ctrl.threads.n.main.tid = mas_gettid(  );
  ctrl.threads.n.main.pid = getpid(  );
  ctrl.threads.n.main.thread = mas_pthread_self(  );
  ctrl.pserver_thread = &ctrl.threads.n.main;


  ctrl.binname = mas_strdup( basename( runpath ) );
  pn = strchr( ctrl.binname, '_' );
  if ( pn && *pn++ && *pn )
    ctrl.progname = mas_strdup( pn );

/* ctrl.pkgname=mas_strdup(_pkgname); */
  return r;
}

int
mas_init_restart_count( mas_options_t * popts )
{
  extern mas_control_t ctrl;
  char name[512];
  char *ren = NULL, *ren0;

  HMSG( "INIT RESTART COUNT %u", ctrl.restart_cnt );
  /* snprintf( name, sizeof( name ), "MAS_%s_%u_RESTART", ctrl.is_client ? "CLIENT" : "SERVER", getpid(  ) ); */
  snprintf( name, sizeof( name ), "MAS_ZOCROMAS_RESTART_%u", getpid(  ) );
  ren0 = ren = getenv( name );
  if ( ren )
  {
    sscanf( ren, "%u", &ctrl.restart_cnt );
    HMSG( "INIT RESTART COUNT (%s) %u", ren, ctrl.restart_cnt );
    ren = strchr( ren, ':' );
    if ( ren )
    {
      unsigned long t;

      ren++;
      sscanf( ren, "%lu", &t );
      ctrl.stamp.first_lts = t;
    }
    ren = strchr( ren, ':' );
    ctrl.stamp.prev_lts = 0;
    if ( ren )
    {
      unsigned long t;

      ren++;
      sscanf( ren, "%lu", &t );
      ctrl.stamp.prev_lts = t;
    }
    MAS_LOG( "@ init [%s='%s']", name, ren0 );
  }
  return 0;
}

int
mas_init( mas_options_t * popts, int argc, char **argv, char **env )
{
  extern mas_control_t ctrl;
  int r = 0;

  HMSG( "INIT" );
  IEVAL( r, mas_pre_init_runpath( argv[0] ) );
  /* IEVAL( r, mas_init_proc( popts ) ); */
  /* uuid BEFORE opt_files !! */
  /* IEVAL( r, mas_init_uuid( popts, NULL ) );      */
  /* IEVAL( r, mas_init_opt_files( popts, NULL ) ); */
  WMSG( "(%d) PRE-INIT", r );
#ifdef MAS_USE_CURSES
  /* // r = mas_init_curses(  ); */
  /* IEVAL( r, mas_init_curses(  ) ); */
#endif


  ctrl.stamp.lts = ( unsigned long ) time( NULL );
  ctrl.stamp.first_lts = ctrl.stamp.lts;
  IEVAL( r, mas_init_restart_count( popts ) );
  MAS_LOG( "@ %u. init @ %lu -> %lu (%lu)", ctrl.restart_cnt, ( unsigned long ) ctrl.stamp.first_lts, ( unsigned long ) ctrl.stamp.lts,
           ( unsigned long ) ctrl.stamp.prev_lts );

  if ( !( mas_init_argv( popts, argc, argv, env ) > 1 ) )
    IEVAL( r, mas_init_env( popts ) );

  /* HMSG( "popts-> argvv.v[0]: %s", popts-> argvv.v[0] ); */
  /* mas_init_message(  ); */
  /* atexit( atexit_fun ); */
  /* IEVAL( r, mas_init_sig(  ) ); */

  /* IEVAL( r, mas_cli_options_init( popts, NULL ) ); */
  /* IEVAL( r, mas_ctrl_init( popts, NULL ) );        */

  return r;
}

static int
mas_init_vplus( mas_options_t * popts, va_list args )
{
  int r = 0;
  int pos = 0;
  typedef int ( *v_t ) ( mas_options_t * popts, const char * *msg );
  v_t fun;

  WMSG( "INIT V+" );
  /* for ( v_t fun = NULL; r >= 0 && !ctrl.is_parent; fun = va_arg( args, v_t ) ) */
  while ( !r && !ctrl.is_parent && ( fun = va_arg( args, v_t ) ) && !ctrl.is_parent )
  {
    const char *msg = NULL;

    HMSG( "(%d) INIT V #%d + %p", r, pos, ( void * ) ( unsigned long ) fun );
    IEVAL( r, ( fun ) ( popts, &msg ) );
    MAS_LOG( "(%d) init + #%d - %s", r, pos, msg ? msg : "-" );
    HMSG( "(%d) INIT V #%d - %s", r, pos, msg ? msg : "-" );
    /* ( ctrl.error_handler ) ( FL, 77 ); */
    pos++;
  }
  HMSG( "(%d) INIT V done (#%d) is_parent:%d", r, pos, ctrl.is_parent );
  MAS_LOG( "(%d) init + done", r );
  return r;
}



int
mas_init_plus( mas_options_t * popts, int argc, char **argv, char **env, ... )
{
  extern mas_control_t ctrl;
  int r = 0;

  WMSG( "INIT+ %s : %s", ctrl.is_server ? "SERVER" : "CLIENT", !ctrl.is_client ? "SERVER" : "CLIENT" );
  IEVAL( r, mas_init( popts, argc, argv, env ) );
  if ( r >= 0 )
  {
    va_list args;

    va_start( args, env );
    IEVAL( r, mas_init_vplus( popts, args ) );
    va_end( args );
  }
  /* IEVAL( r, mas_post_init( popts ) ); */
  MAS_LOG( "(%d) init done e%d", r, errno );
  HMSG( "(%d)INIT %s", r, r < 0 ? "FAIL" : "OK" );
  ctrl.inited = 1;
  return r;
}

void
mas_destroy( mas_options_t * popts )
{
  extern mas_control_t ctrl;
  char sppid[64] = "";


  snprintf( sppid, sizeof( sppid ), "%lu", ( unsigned long ) getppid(  ) );
  if ( *sppid )
  {
    if ( !ctrl.opts_saved )
      mas_opts_save_user( popts, NULL, ctrl.progname ? ctrl.progname : "Unknown" );
    if ( !ctrl.opts_saved_plus )
      mas_opts_save_user_plus( popts, NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", sppid, NULL );
  }
  WMSG( "DESTROY" );
  MAS_LOG( "destroy server" );
  /* mutex?? */
#ifdef MAS_USE_CURSES
  if ( ctrl.is_client )
    mas_close_curses(  );
#endif
  if ( popts->argvv.v )
  {
    /* HMSG( "destroy, restart:%d [%s]", ctrl.restart, popts-> argvv.v[0] ); */
    tMSG( ">>>>> %s %s", popts->argvv.v[0], *( &popts->argvv.v[1] ) );
    if ( ctrl.restart )
    {
      int r = 0;

      HMSG( "DESTROY, RESTART" );
/* see mas_control_data.c mas_control_types.h etc. */
      WMSG( "execvp %s %s", popts->argvv.v[0], popts->argvv.v[1] );
      ctrl.restart_cnt++;
      {
        char val[512];
        char name[512];

        snprintf( name, sizeof( name ), "MAS_ZOCROMAS_RESTART_%u", getpid(  ) );
        snprintf( val, sizeof( val ), "%u:%lu:%lu", ctrl.restart_cnt, ( unsigned long ) ctrl.stamp.first_lts,
                  ( unsigned long ) ctrl.stamp.lts );
        HMSG( "RESTART : %s='%s'", name, val );
        setenv( name, val, 1 );
      }
      IEVAL( r, execvp( popts->argvv.v[0], &popts->argvv.v[0] ) );
      HMSG( "FAIL restart %d %s", r, popts->argvv.v[0] );
      P_ERR;
    }
  }
  mas_opts_destroy( popts );

  ctrl.log_disabled = 1;

  if ( ctrl.logpath )
    mas_free( ctrl.logpath );
  ctrl.logpath = NULL;

  if ( ctrl.binname )
    mas_free( ctrl.binname );
  ctrl.binname = NULL;

  if ( ctrl.progname )
    mas_free( ctrl.progname );
  ctrl.progname = NULL;

  if ( ctrl.exepath )
    mas_free( ctrl.exepath );
  ctrl.exepath = NULL;

  if ( ctrl.exename )
    mas_free( ctrl.exename );
  ctrl.exename = NULL;

  mas_ctrl_destroy(  );

  MAS_LOG( "destroy done" );
  mas_logger_delete( 1 );
  HMSG( "DESTROY DONE" );
#ifdef MAS_TRACEMEM
  {
    /* int k; */
    extern unsigned long memory_balance;

    /* k = print_memlist( ctrl.msgfile, FL ); */
    if ( 0 )
    {
      char *bad_mem;

      bad_mem = mas_malloc( 3735928495 );
      if ( bad_mem )
        strcpy( bad_mem, "DEAD BEAF" );
    }
    FMSG( "destroy, memory_balance:%ld (%lX);", memory_balance, memory_balance );
    print_memlist_any( FL, 4, ctrl.stderrfile, ctrl.old_stderrfile, ctrl.msgfile, stderr );
    fflush( ctrl.msgfile );
  }
#endif
}

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  char name[512];

  /* char *value = NULL; */

  /* fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno ); */
  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;

  snprintf( name, sizeof( name ), "MAS_ZOCROMAS_RESTART_%u", ctrl.threads.n.main.pid );
  /* value = getenv( name ); */
  /* fprintf( stderr, "******************** [%s='%s'] /CONSTRUCTOR %s e%d\n", name, value, __FILE__, errno ); */
  /* ctrl.is_server = 0; */
  /* ctrl.is_client = 0; */
}

__attribute__ ( ( destructor ) )
     static void master_destructor( void )
{
  /* fprintf( stderr, "******************** DESTRUCTOR %s e%d\n", __FILE__, errno ); */
}
