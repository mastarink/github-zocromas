#include <mastar/wrap/mas_std_def.h>
/* #include "mas_basic_def.h" */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


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

static int
mas_pre_init( char *runpath )
{
  int r = 0;
  const char *pn;

  ctrl.status = MAS_STATUS_START;
  HMSG( "PRE-INIT" );
  ctrl.main_tid = mas_gettid(  );
  ctrl.start_time = mas_double_time(  );
  /* ctrl.stamp.lts = ( unsigned long ) time( NULL ); */
  ctrl.stamp.first_lts = 0;
  ctrl.status = MAS_STATUS_INIT;
  ctrl.main_pid = getpid(  );
  ctrl.binname = mas_strdup( basename( runpath ) );
  {
    char lexe[256];
    struct stat sb;
    char *linkname;

    HMSG( "LINKNAME ?" );
    sprintf( lexe, "/proc/%u/exe", getpid(  ) );
    if ( lstat( lexe, &sb ) >= 0 )
    {
      size_t sz;

      sz = ( sb.st_size ? sb.st_size : 512 ) + 1;
      linkname = mas_malloc( sz );
      r = readlink( lexe, linkname, sz );
      if ( r >= 0 )
      {
        linkname[sz] = '\0';
        HMSG( "(%s) [%lu] LINKNAME [%d]: '%s'", lexe, sz, r, linkname );
      }
      else
      {
        P_ERR;
        HMSG( "(%s) LINKNAME: ---", lexe );
      }
      ctrl.exepath = linkname;
    }
  }
  pn = strchr( ctrl.binname, '_' );
  if ( pn && *pn++ && *pn )
    ctrl.progname = mas_strdup( pn );
  HMSG( "PPID: %u; BASH: %s", getppid(  ), getenv( "MAS_PID_AT_BASHRC" ) );
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
  HMSG( "(%d) PRE-INIT", r );
  return r;
}

static int
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

    snprintf( namebuf, sizeof( namebuf ), "/%s.%s.%lu.%u.log", ctrl.is_client ? "client" : "server", ctrl.is_parent ? "parent" : "child",
              ctrl.stamp.first_lts, getpid(  ) );
    ctrl.logpath = mas_strdup( opts.logdir );
    ctrl.logpath = mas_strcat_x( ctrl.logpath, namebuf );
    HMSG( "LOG: %s", ctrl.logpath );
  }
  else
  {
    EMSG( "logdir not set" );
  }
  if ( !ctrl.is_parent )
  {
    if ( r >= 0 && opts.msgfilename )
    {
      HMSG( "MESSAGES to %s", opts.msgfilename );
      mas_msg_set_file( opts.msgfilename );
      MFP( "\x1b[H\x1b[2J" );
    }
    if ( r >= 0 )
      r = mas_init_message(  );
  }
  MAS_LOG( "(%d) init done", r );
  HMSG( "(%d) POST INIT DONE", r );
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
  return 0;
}

int
mas_init( int argc, char **argv, char **env )
{
  int r = 0;

  HMSG( "INIT" );
  ctrl.stamp.lts = ( unsigned long ) time( NULL );
  ctrl.stamp.first_lts = ctrl.stamp.lts;
  if ( r >= 0 )
    r = mas_init_restart_count(  );
  HMSG( "(%d) INIT %d", r, __LINE__ );
  MAS_LOG( "@ %u. init @ %lu -> %lu (%lu)", ctrl.restart_cnt, ctrl.stamp.first_lts, ctrl.stamp.lts, ctrl.stamp.prev_lts );
  /* if ( ctrl.is_server ) */

  if ( r >= 0 && !( mas_init_argv( argc, argv, env ) > 1 ) )
    r = mas_init_env(  );
  HMSG( "(%d) INIT %d", r, __LINE__ );

  /* HMSG( "opts.argv[0]: %s", opts.argv[0] ); */
  /* mas_init_message(  ); */
  /* atexit( atexit_fun ); */
  if ( r >= 0 )
    r = mas_init_sig(  );
  HMSG( "(%d) INIT %d", r, __LINE__ );

  r = mas_cli_options( opts.argc, opts.argv );
  HMSG( "(%d) INIT %d", r, __LINE__ );
  if ( r >= 0 )
  {
    ctrl.argv_nonoptind = r;
    r = mas_ctrl_init( &opts );
    HMSG( "(%d) INIT %d", r, __LINE__ );
  }

  return r;
}

static int
mas_init_vplus( va_list args )
{
  int r = 0;
  int pos = 0;
  typedef int ( *v_t ) ( void );
  v_t fun;

  /* for ( v_t fun = NULL; r >= 0 && !ctrl.is_parent; fun = va_arg( args, v_t ) ) */
  while ( r >= 0 && !ctrl.is_parent && ( fun = va_arg( args, v_t ) ) )
  {
    MAS_LOG( "(%d) init + #%d", r, pos );
    r = ( fun ) (  );
    HMSG( "(%d) INIT %d", r, __LINE__ );
    pos++;
  }
  MAS_LOG( "(%d) init + done", r );
  return r;
}

int
mas_init_plus( int argc, char **argv, char **env, ... )
{
  int r = 0;
  va_list args;

  HMSG( "INIT+ %s : %s", ctrl.is_server ? "SERVER" : "CLIENT", !ctrl.is_client ? "SERVER" : "CLIENT" );
  if ( r >= 0 )
    r = mas_pre_init( argv[0] );
  HMSG( "(%d) INIT %d", r, __LINE__ );
  if ( r >= 0 )
    r = mas_init( argc, argv, env );
  HMSG( "(%d) INIT %d", r, __LINE__ );
  {
    va_start( args, env );
    if ( r >= 0 )
      r = mas_init_vplus( args );
    HMSG( "(%d) INIT %d", r, __LINE__ );
    va_end( args );
  }
  if ( r >= 0 )
    r = mas_post_init(  );
  HMSG( "(%d) INIT %d", r, __LINE__ );
  HMSG( "INIT %s", r < 0 ? "FAIL" : "OK" );
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
    tMSG( ">>>>> %s %s", opts.argv[0], *( &opts.argv[1] ) );
    if ( ctrl.restart )
    {
      int r = 0;

      HMSG( "DESTROY, RESTART" );
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

  mas_ctrl_destroy(  );

  MAS_LOG( "destroy done" );
  mas_log_clean_queue(  );
  HMSG( "DESTROY DONE" );
#ifdef MAS_TRACEMEM
  {
    /* int k; */
    extern unsigned long memory_balance;

    /* k = print_memlist( ctrl.msgfile, FL ); */
    FMSG( "destroy, memory_balance:%ld;", memory_balance );
    /* k = print_memlist( ctrl.msgfile, FL ); */
    if ( !ctrl.stderrfile || print_memlist( ctrl.stderrfile, FL ) < 0 )
      if ( !ctrl.old_stderrfile || print_memlist( ctrl.old_stderrfile, FL ) < 0 )
        if ( print_memlist( ctrl.msgfile, FL ) < 0 )
          print_memlist( stderr, FL );
    /* k = print_memlist( ctrl.msgfile, FL ); */
    fflush( ctrl.msgfile );
  }
#endif
}

__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  char name[512];
  char *value = NULL;

  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;

  snprintf( name, sizeof( name ), "MAS_ZOCROMAS_RESTART_%u", ctrl.main_pid );
  value = getenv( name );
  if ( ctrl.stderrfile )
    fprintf( ctrl.stderrfile, "******************** [%s='%s'] CONSTRUCTOR %s\n", name, value, __FILE__ );
  /* ctrl.is_server = 0; */
  /* ctrl.is_client = 0; */
}

__attribute__ ( ( destructor ) )
     static void master_destructor( void )
{
  if ( ctrl.stderrfile )
    fprintf( ctrl.stderrfile, "******************** DESTRUCTOR %s\n", __FILE__ );
}
