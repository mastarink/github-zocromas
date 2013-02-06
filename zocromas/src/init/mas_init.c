#include "mas_basic_def.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib_thread.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

/* #include "mas_common.h" */
#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include "control/inc/mas_control.h"
#include "mas_opts.h"

#include <mastar/msg/mas_curses.h>
#include <mastar/variables/mas_variables.h>

#include "listener/inc/mas_listener_control.h"
#include "cli/inc/mas_cli_options.h"

#include "init/inc/mas_init.h"


int use_curses = 0;
WINDOW *w_rootwin = NULL, *w_main0 = NULL, *w_main = NULL, *w_listen0 = NULL, *w_listen = NULL, *w_trans0 = NULL, *w_trans =
      NULL, *w_other0 = NULL, *w_other = NULL;


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


static void
mas_init_message( void )
{
  if ( use_curses )
  {
    IMSG( "[%s] %s V.%s built at %s : %lx : %lx : %lu; (%s) pid=%lx ; tid:%u [%lx]", ctrl.progname, PACKAGE_NAME, PACKAGE_VERSION,
          MAS_C_DATE, ( unsigned long ) ctrl.stamp.vdate, ( unsigned long ) ctrl.stamp.vtime, ( unsigned long ) ctrl.stamp.vts,
          ctrl.stamp.vtsc, ( unsigned long ) ctrl.main_pid, ( unsigned ) ctrl.main_tid, ( unsigned long ) ctrl.main_thread );
  }
  else
  {
    IMSG( "\x1b[100;27;1;32m [%s] %s V.%s built\x1b[0;100m at %s : %lx : %lx : %lu; (%s) pid=%lx ; tid:%u [%lx] \x1b[0m",
          ctrl.progname, PACKAGE_NAME, PACKAGE_VERSION, MAS_C_DATE, ( unsigned long ) ctrl.stamp.vdate,
          ( unsigned long ) ctrl.stamp.vtime, ( unsigned long ) ctrl.stamp.vts, ctrl.stamp.vtsc, ( unsigned long ) ctrl.main_pid,
          ( unsigned ) ctrl.main_tid, ( unsigned long ) ctrl.main_thread );
}}

void
mas_pre_init( int argc, char **argv, char **env )
{
  const char *pn;

  ctrl.status = MAS_STATUS_INIT;
  ctrl.binname = mas_strdup( basename( argv[0] ) );
  pn = strchr( ctrl.binname, '_' );
  if ( pn )
  {
    pn++;
    if ( *pn )
      ctrl.progname = mas_strdup( pn );
  }
}

void
mas_init( void ( *atexit_fun ) ( void ), int initsig, int argc, char **argv, char **env )
{
  ctrl.stamp.lts = ( unsigned long ) time( NULL );
  ctrl.stamp.first_lts = 0;
  {
    char name[512];
    char *ren = NULL, *ren0;

    snprintf( name, sizeof( name ), "MAS_%s_%u_RESTART", ctrl.is_client ? "CLIENT" : "SERVER", getpid(  ) );
    ren0 = ren = getenv( name );
    if ( ren )
    {
      sscanf( ren, "%u", &ctrl.restart_cnt );
      ren = strchr( ren, ':' );
      if ( ren )
      {
        ren++;
        sscanf( ren, "%lu", &ctrl.stamp.first_lts );
      }
      ren = strchr( ren, ':' );
      ctrl.stamp.prev_lts = 0;
      if ( ren )
      {
        ren++;
        sscanf( ren, "%lu", &ctrl.stamp.prev_lts );
      }
      MAS_LOG( "@ init [%s='%s']", name, ren0 );
    }
  }
  if ( !ctrl.stamp.first_lts )
    ctrl.stamp.first_lts = ctrl.stamp.lts;
  MAS_LOG( "@ %u. init @ %lu -> %lu (%lu)", ctrl.restart_cnt, ctrl.stamp.first_lts, ctrl.stamp.lts, ctrl.stamp.prev_lts );
  /* if ( ctrl.is_server ) */
  ctrl.main_pid = getpid(  );

  if ( !( mas_init_argv( argc, argv, env ) > 1 ) )
    mas_init_env(  );

  /* HMSG( "opts.argv[0]: %s\n", opts.argv[0] ); */
  mas_init_message(  );
  mas_ctrl_init( &opts );

  atexit( atexit_fun );
  if ( initsig )
    mas_init_sig(  );

  ctrl.argv_nonoptind = mas_cli_options( opts.argc, opts.argv );
  /* mMSG( "ARGV_NONOPTIND :%d", ctrl.argv_nonoptind ); */

  if ( !opts.hosts_num && 0 )
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
/*  
  if ( !opts.logdir )
    opts.logdir = mas_strdup( ".........." );
*/
  if ( opts.logdir )
  {
    char namebuf[512];

    snprintf( namebuf, sizeof( namebuf ), ctrl.is_client ? "/client.%lu.%u.log" : "/server.%lu.%u.log", ctrl.stamp.first_lts,
              getpid(  ) );
    ctrl.logpath = mas_strdup( opts.logdir );
    ctrl.logpath = mas_strcat_x( ctrl.logpath, namebuf );
  }
  else
  {
    EMSG( "logdir not set" );
  }

  /* ctrl.listening_max = opts.hosts_num; */
  MAS_LOG( "init done, %d hosts", opts.hosts_num );
}

void
mas_destroy( void )
{
  MAS_LOG( "destroy server" );
  /* mutex?? */
  if ( ctrl.is_client )
    mas_close_curses(  );

  if ( opts.argv )
  {
    HMSG( "(%d) destroy, restart:%d [%s]\n", __LINE__, ctrl.restart, opts.argv[0] );
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

        snprintf( name, sizeof( name ), "MAS_%s_%u_RESTART", ctrl.is_client ? "CLIENT" : "SERVER", getpid(  ) );
        snprintf( val, sizeof( val ), "%u:%lu:%lu", ctrl.restart_cnt, ctrl.stamp.first_lts, ctrl.stamp.lts );
        setenv( name, val, 1 );
      }
      r = execvp( opts.argv[0], &opts.argv[0] );
      HMSG( "FAIL restart %d %s\n", r, opts.argv[0] );
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
#ifdef MAS_TRACEMEM
  {
    extern unsigned long memory_balance;

    FMSG( "destroy, memory_balance:%ld;", memory_balance );
    print_memlist( FL );
  }
#endif
}
