#include <mastar/wrap/mas_std_def.h>
/* #include "mas_basic_def.h" */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* #include <pthread.h> */

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>


#include <mastar/control/mas_control.h>

#ifdef MAS_USE_CURSES
#  include <mastar/msg/mas_curses.h>
#endif
#include <mastar/variables/mas_variables.h>

#include "mas_opts.h"
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
mas_init_argv( int argc, char **argv, char **env )
{
  ctrl.launcherv = argv;
  ctrl.launcherc = argc;
  ctrl.launchere = env;
  /* if ( argc > 1 ) */
  {
    for ( int ia = 0; ia < argc; ia++ )
    {
      opts.argc = mas_add_argv_arg( opts.argc, &opts.argv, argv[ia] );
    }
    /* for ( int ia = 0; ia < argc; ia++ )                              */
    /* {                                                                */
    /*   mMSG( "A: %d of %d. arg:'%s'", ia, opts.argc, opts.argv[ia] ); */
    /* }                                                                */
  }
  return opts.argc;
}

int
mas_init_env(  )
{
  char *seopts = NULL;

  if ( opts.env_optsname && ( seopts = getenv( opts.env_optsname ) ) )
  {
    mMSG( "E: %s='%s'", opts.env_optsname, seopts );
    /* opts.argc = mas_make_argv( seopts, opts.argc, &opts.argv, 0 ); */
    opts.argc = mas_add_argv_args( opts.argc, &opts.argv, seopts, 0 );
    /* for ( int ia = 0; ia < opts.argc; ia++ )                         */
    /* {                                                                */
    /*   mMSG( "E: %d of %d. arg:'%s'", ia, opts.argc, opts.argv[ia] ); */
    /* }                                                                */
  }
  /* opts.argc = mas_make_argv( "-P5001 -H192.168.71.2 -H192.168.71.2:5002 -H192.168.71.2:5003", opts.argc, &opts.argv, 0 ); */
  return 0;
}


static int
mas_init_message( void )
{
#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    IMSG( "[%s] %s V.%s built at %s : %lx : %lx : %lu; (%s)", ctrl.progname, PACKAGE_NAME, PACKAGE_VERSION, MAS_C_DATE,
          ( unsigned long ) ctrl.stamp.vdate, ( unsigned long ) ctrl.stamp.vtime, ( unsigned long ) ctrl.stamp.vts, ctrl.stamp.vtsc );
    IMSG( "[%s] pid=%u(x%x) ; tid:%u [%lx]", ctrl.progname, ctrl.main_pid, ctrl.main_pid, ( unsigned ) ctrl.main_tid,
          ( unsigned long ) ctrl.main_thread );
  }
  else
#endif
  {
    IMSG( "\x1b[100;27;1;32m [%s] %s V.%s built\x1b[0;100m at %s : %lx : %lx : %lu; (%s) \x1b[0m", ctrl.progname, PACKAGE_NAME,
          PACKAGE_VERSION, MAS_C_DATE, ( unsigned long ) ctrl.stamp.vdate, ( unsigned long ) ctrl.stamp.vtime,
          ( unsigned long ) ctrl.stamp.vts, ctrl.stamp.vtsc );
    IMSG( "\x1b[100;27;1;32m [%s] pid=[\x1b[47;31m %u \x1b[100;32m](%x) ; tid:%u [%lx] \x1b[0m", ctrl.progname, ctrl.main_pid,
          ctrl.main_pid, ( unsigned ) ctrl.main_tid, ( unsigned long ) ctrl.main_thread );
  }
  return 0;
}

int
mas_pre_init( int argc, char **argv, char **env )
{
  int r = 0;
  const char *pn;

  HMSG( "PRE-INIT" );
  /* ctrl.stamp.lts = ( unsigned long ) time( NULL ); */
  ctrl.stamp.first_lts = 0;
  ctrl.status = MAS_STATUS_INIT;
  ctrl.binname = mas_strdup( basename( argv[0] ) );
  ctrl.main_pid = getpid(  );
  pn = strchr( ctrl.binname, '_' );
  if ( pn )
  {
    pn++;
    if ( *pn )
      ctrl.progname = mas_strdup( pn );
  }
  HMSG( "PPID: %u", getppid(  ) );
  HMSG( "BASH: %s", getenv( "MAS_PID_AT_BASHRC" ) );
  MAS_LOG( "PPID: %u BASH: %s", getppid(  ), getenv( "MAS_PID_AT_BASHRC" ) );
  if ( r >= 0 )
  {
    r = mas_opts_restore_plus( NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", getenv( "MAS_PID_AT_BASHRC" ), NULL );
    if ( r <= 0 )
      r = mas_opts_restore( NULL, ctrl.progname ? ctrl.progname : "Unknown" );
#ifdef MAS_USE_CURSES
    /* r = mas_init_curses(  ); */
#endif
  }
  return r;
}

int
mas_post_init( void )
{
  int r = 0;

  HMSG( "POST-INIT" );
#if 0
  if ( r >= 0 && !opts.hosts_num )
  {
    char *defhost = NULL;

    if ( opts.env_hostname )
      defhost = getenv( opts.env_hostname );
    if ( !defhost )
      defhost = "localhost";
    opts.hosts_num = mas_add_argv_arg( opts.hosts_num, &opts.hosts, defhost );
    /* for ( int ih = 0; ih <= opts.hosts_num; ih++ )                     */
    /* {                                                                  */
    /*   thMSG( "@@@@@@ %d. host %s (%s)", ih, opts.hosts[ih], defhost ); */
    /* }                                                                  */
  }
#endif
/*  
  if ( !opts.logdir )
    opts.logdir = mas_strdup( ".........." );
*/
  if ( r >= 0 && opts.logdir )
  {
    char namebuf[512];

    snprintf( namebuf, sizeof( namebuf ), "/%s.%lu.%u.log" , ctrl.is_client ? "client" : "server", ctrl.stamp.first_lts, getpid(  ) );
    ctrl.logpath = mas_strdup( opts.logdir );
    ctrl.logpath = mas_strcat_x( ctrl.logpath, namebuf );
  }
  else
  {
    EMSG( "logdir not set" );
  }
  HMSG( "PIDSDIR:%s", opts.pidsdir );
  if ( r >= 0 && opts.pidsdir )
  {
    char namebuf[512];
    char *pidpath;

    snprintf( namebuf, sizeof( namebuf ), "/%s.%u.pid", ctrl.is_client ? "client" : "server", getppid(  ) );
    pidpath = mas_strdup( opts.pidsdir );
    pidpath = mas_strcat_x( pidpath, namebuf );
    HMSG( "PIDPATH:%s", pidpath );
    if ( pidpath )
    {
      FILE *f;

      f = fopen( pidpath, "w" );
      if ( f )
      {
        int w;

        /* w = fwrite( pidpath, 1, strlen( pidpath ), f ); */
        w = fprintf( f, "%u", getpid(  ) );
        HMSG( "PIDW:%d", w );
        fclose( f );
      }
    }
    mas_free( pidpath );
  }

  /* ctrl.listening_max = opts.hosts_num; */
  MAS_LOG( "(%d) init done, %d hosts", r, opts.hosts_num );

  if ( opts.msgfilename )
  {
    HMSG( "MESSAGES to %s", opts.msgfilename );
    mas_msg_set_file( opts.msgfilename );
    MFP( "\x1b[H\x1b[2J" );
  }
  r = mas_init_message(  );
  return r;
}

int
mas_init_restart_count( void )
{
  char name[512];
  char *ren = NULL, *ren0;

  /* snprintf( name, sizeof( name ), "MAS_%s_%u_RESTART", ctrl.is_client ? "CLIENT" : "SERVER", getpid(  ) ); */
  snprintf( name, sizeof( name ), "MAS_ZOCROMAS_RESTART_%u", getpid(  ) );
  ren0 = ren = getenv( name );
  if ( ren )
  {
    fprintf( stderr, "@@@@@@@@ A:%u : %lu\n", ctrl.restart_cnt, ctrl.stamp.first_lts );
    sscanf( ren, "%u", &ctrl.restart_cnt );
    ren = strchr( ren, ':' );
    if ( ren )
    {
      ren++;
      sscanf( ren, "%lu", &ctrl.stamp.first_lts );
    }
    fprintf( stderr, "@@@@@@@@ B:%u : %lu\n", ctrl.restart_cnt, ctrl.stamp.first_lts );
    ren = strchr( ren, ':' );
    ctrl.stamp.prev_lts = 0;
    if ( ren )
    {
      ren++;
      sscanf( ren, "%lu", &ctrl.stamp.prev_lts );
    }
    MAS_LOG( "@ init [%s='%s']", name, ren0 );
    if ( !ctrl.stamp.first_lts )
      ctrl.stamp.first_lts = ctrl.stamp.lts;
  }
  return 0;
}

int
mas_init( int argc, char **argv, char **env )
{
  int r = 0;

  HMSG( "INIT" );
  ctrl.stamp.lts = ( unsigned long ) time( NULL );
  if ( r >= 0 )
    r = mas_init_restart_count(  );
  MAS_LOG( "@ %u. init @ %lu -> %lu (%lu)", ctrl.restart_cnt, ctrl.stamp.first_lts, ctrl.stamp.lts, ctrl.stamp.prev_lts );
  /* if ( ctrl.is_server ) */

  if ( r >= 0 && !( mas_init_argv( argc, argv, env ) > 1 ) )
    r = mas_init_env(  );

  /* HMSG( "opts.argv[0]: %s", opts.argv[0] ); */
  /* mas_init_message(  ); */
  /* atexit( atexit_fun ); */
  if ( r >= 0 )
    r = mas_init_sig(  );

  ctrl.argv_nonoptind = mas_cli_options( opts.argc, opts.argv );
  if ( r >= 0 )
    mas_ctrl_init( &opts );

  return r;
}

int
mas_init_plus( int argc, char **argv, char **env, ... )
{
  int r = 0;
  va_list args;

  ctrl.main_tid = mas_gettid(  );
  HMSG( "INIT+ %s : %s", ctrl.is_server ? "SERVER" : "CLIENT", !ctrl.is_client ? "SERVER" : "CLIENT" );
  ctrl.status = MAS_STATUS_START;
  ctrl.start_time = mas_double_time(  );
  /* ctrl.is_client / ctrl.is_server set at the beginning of mas_init_client / mas_init_server */
  /* ctrl.is_server = is_server;       */
  /* ctrl.is_client = !ctrl.is_server; */
#ifndef MAS_CLIENT_LOG
  if ( ctrl.is_client )
    ctrl.log_disabled = 1;
#endif
  r = mas_pre_init( argc, argv, env );
  if ( r >= 0 )
    r = mas_init( argc, argv, env );
  {
    typedef int ( *v_t ) ( void );
    v_t fun;

    va_start( args, env );
    while ( r >= 0 && ( fun = va_arg( args, v_t ) ) )
      r = ( fun ) (  );
    va_end( args );
  }
  if ( r >= 0 )
    r = mas_post_init(  );
  return r;
}

void
mas_destroy( void )
{
  MAS_LOG( "destroy server" );
  /* mutex?? */
#ifdef MAS_USE_CURSES
  if ( ctrl.is_client )
    mas_close_curses(  );
#endif
  if ( opts.argv )
  {
    /* HMSG( "destroy, restart:%d [%s]", ctrl.restart, opts.argv[0] ); */
    HMSG( "DESTROY, restart:%d", ctrl.restart );
    tMSG( ">>>>> %s %s", opts.argv[0], *( &opts.argv[1] ) );
    if ( ctrl.restart )
    {
      int r = 0;

/* see mas_control_data.c mas_control_types.h etc. */
      HMSG( "execvp %s %s", opts.argv[0], opts.argv[1] );
      ctrl.restart_cnt++;
      {
        char val[512];
        char name[512];

        snprintf( name, sizeof( name ), "MAS_ZOCROMAS_RESTART_%u", getpid(  ) );
        snprintf( val, sizeof( val ), "%u:%lu:%lu", ctrl.restart_cnt, ctrl.stamp.first_lts, ctrl.stamp.lts );
        setenv( name, val, 1 );
      }
      r = execvp( opts.argv[0], &opts.argv[0] );
      HMSG( "FAIL restart %d %s", r, opts.argv[0] );
      P_ERR;
    }
  }
  mas_opts_destroy(  );

  mas_variables_delete( ctrl.hostvars );
  ctrl.hostvars = NULL;

  ctrl.log_disabled = 1;
  if ( ctrl.logpath )
    mas_free( ctrl.logpath );
  if ( ctrl.binname )
    mas_free( ctrl.binname );
  if ( ctrl.progname )
    mas_free( ctrl.progname );
  ctrl.logpath = NULL;
  mas_ctrl_destroy(  );

  MAS_LOG( "destroy done" );
  MAS_LOG( "destroy done" );
  mas_log_clean_queue(  );
  HMSG( "DESTROY DONE" );
#ifdef MAS_TRACEMEM
  {
    extern unsigned long memory_balance;

    FMSG( "destroy, memory_balance:%ld;", memory_balance );
    print_memlist( FL );
  }
#endif
}

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  char name[512];
  char *value = NULL;

  snprintf( name, sizeof( name ), "MAS_ZOCROMAS_RESTART_%u", getpid(  ) );
  value = getenv( name );
  fprintf( stderr, "******************** [%s='%s'] CONSTRUCTOR %s\n", name, value, __FILE__ );
  /* ctrl.is_server = 0; */
  /* ctrl.is_client = 0; */
}

__attribute__ ( ( destructor ) )
     static void master_destructor( void )
{
  fprintf( stderr, "******************** DESTRUCTOR %s\n", __FILE__ );
}
