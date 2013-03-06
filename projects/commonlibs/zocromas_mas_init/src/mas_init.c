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
  WMSG( "INIT ARGV" );
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
  extern unsigned long __MAS_LINK_DATE__;
  extern unsigned long __MAS_LINK_TIME__;
  extern unsigned long __MAS_LINK_TIMESTAMP__;

  WMSG( "INIT MESSAGE" );

#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    IMSG( "[%s] %s V.%s built at %s : %lx : %lx : %lu; #%u (%s)", ctrl.progname, PACKAGE_NAME, PACKAGE_VERSION, MAS_C_DATE,
          ( unsigned long ) ( &__MAS_LINK_DATE__ ), ( unsigned long ) ( &__MAS_LINK_TIME__ ), ( unsigned long ) ( &__MAS_LINK_TIMESTAMP__ ),
          ctrl.restart_cnt, ctrl.stamp.vtsc );
    IMSG( "[%s] pid=%u(x%x) ; tid:%u [%lx]", ctrl.progname, ctrl.threads.n.main.pid, ctrl.threads.n.main.pid,
          ( unsigned ) ctrl.threads.n.main.tid, ( unsigned long ) ctrl.threads.n.main.thread );
  }
  else
#endif
  {
    IMSG( "-" );
    IMSG( " [%s] %s V.%s built at %s : %lx : %lx : %lu; #%u (%s) ", ctrl.progname, PACKAGE_NAME,
          PACKAGE_VERSION, MAS_C_DATE, ( unsigned long ) ( &__MAS_LINK_DATE__ ), ( unsigned long ) ( &__MAS_LINK_TIME__ ),
          ( unsigned long ) ( &__MAS_LINK_TIMESTAMP__ ), ctrl.restart_cnt, ctrl.stamp.vtsc );
    IMSG( " [%s] pid=[ %u ](%x) ; tid:%u [%lx] ", ctrl.progname, ctrl.threads.n.main.pid,
          ctrl.threads.n.main.pid, ( unsigned ) ctrl.threads.n.main.tid, ( unsigned long ) ctrl.threads.n.main.thread );
    IMSG( "-" );
    /* IMSG( "\x1b[100;27;1;32m [%s] %s V.%s built\x1b[0;100m at %s : %lx : %lx : %lu; (%s) \x1b[0m", ctrl.progname, PACKAGE_NAME,          */
    /*       PACKAGE_VERSION, MAS_C_DATE, ( unsigned long ) ( &__MAS_LINK_DATE__ ), ( unsigned long ) ( &__MAS_LINK_TIME__ ),               */
    /*       ( unsigned long ) ( &__MAS_LINK_TIMESTAMP__ ), ctrl.stamp.vtsc );                                                              */
    /* IMSG( "\x1b[100;27;1;32m [%s] pid=[\x1b[47;31m %u \x1b[100;32m](%x) ; tid:%u [%lx] \x1b[0m", ctrl.progname, ctrl.threads.n.main.pid, */
    /*       ctrl.threads.n.main.pid, ( unsigned ) ctrl.threads.n.main.tid, ( unsigned long ) ctrl.threads.n.main.thread );                 */
  }
  return 0;
}

static int
error_handler_at_init( const char *func, int line, int rcode, int ierrno, const char *fmt, const char *msg )
{
  if ( !fmt )
    fmt = " r #%d [%s]";

  /* va_list args; */
  /* int masierrno; */

  HMSG( "%d:%s ; r:%d ; msg : %s", line, func, rcode, msg ? msg : "-" );
  mas_error( func, line, ierrno, fmt, rcode, msg ? msg : "-" );
  mas_log( func, line, ierrno, fmt, rcode, msg ? msg : "-" );
  /* va_start( args, rcode );                  */
  /* while ( masierrno = va_arg( args, int ) ) */
  /* {                                         */
  /*   const char *fmt;                        */
  /* }                                         */
  /* va_end( args );                           */
  return rcode;
}

static int
mas_pre_init_proc( void )
{
  int r = 0;
  char lexe[256];
  struct stat sb;
  char *linkname;

  WMSG( "PRE-INIT" );
  sprintf( lexe, "/proc/%u/exe", getpid(  ) );
  /* if ( lstat( lexe, &sb ) >= 0 ) */
  IEVAL( r, lstat( lexe, &sb ) );
  if ( !( r < 0 ) )
  {
    size_t sz;

    sz = ( sb.st_size ? sb.st_size : 512 ) + 1;
    linkname = mas_malloc( sz );
    /* r = readlink( lexe, linkname, sz ); */
    IEVAL( r, readlink( lexe, linkname, sz ) );
    if ( !( r < 0 ) )
    {
      linkname[sz] = '\0';
      WMSG( "(%s) [%u] LINKNAME [%d]: '%s'", lexe, ( unsigned ) sz, r, linkname );
    }
    ctrl.exepath = linkname;
    ctrl.exename = mas_strdup( basename( ctrl.exepath ) );
  }
  return r;
}

static int
mas_pre_init_opt_files( void )
{
  int r = 0;
  char sppid[64] = "";

  WMSG( "INIT OPT FILES" );
  HMSG( "PPID: %u; BASH VAR: %s", getppid(  ), getenv( "MAS_PID_AT_BASHRC" ) );
  MAS_LOG( "PPID: %u BASH VAR: %s", getppid(  ), getenv( "MAS_PID_AT_BASHRC" ) );
  {
    snprintf( sppid, sizeof( sppid ), "%lu", ( unsigned long ) getppid(  ) );
  }
  if ( *sppid )
  {
    /* const char *name = ctrl.progname; */
    const char *name = ctrl.exename;

#ifdef MAS_ONE_OF_CONFIGS
    int rone = 0;

    /* IEVAL_OPT( rone, mas_opts_restore_plus( NULL, name, ".", getenv( "MAS_PID_AT_BASHRC" ), NULL ) ); */
    IEVAL_OPT( rone, mas_opts_restore_plus( NULL, name, ".", sppid, NULL ) );
    if ( !( rone > 0 ) )
    {
      IEVALM( r, mas_opts_restore( NULL, name ), "(%d) no opt file(s) for prog: '%s'", name );
    }
#else
    int rzero = 0;
    int rone = 0;
    int rtwo = 0;

    IEVAL_OPT( rzero, mas_opts_restore_zero( name ) );
    IEVAL_OPT( rone, mas_opts_restore( NULL, name ) );
    /* IEVAL_OPT( rtwo, mas_opts_restore_plus( NULL, name, ".", getenv( "MAS_PID_AT_BASHRC" ), NULL ) ); */
    IEVAL_OPT( rtwo, mas_opts_restore_plus( NULL, name, ".", sppid, NULL ) );
#endif
  }
  return r;
}

static int
mas_pre_init_runpath( char *runpath )
{
  int r = 0;
  const char *pn;

  ctrl.status = MAS_STATUS_START;
  ctrl.error_handler = error_handler_at_init;
  ctrl.threads.n.main.tid = mas_gettid(  );

  ctrl.start_time = mas_double_time(  );
  ctrl.stamp.start_time = ( unsigned long ) time( NULL );
  /* ctrl.stamp.lts = ( unsigned long ) time( NULL ); */
  ctrl.stamp.first_lts = 0;
  ctrl.status = MAS_STATUS_INIT;
  ctrl.threads.n.main.pid = getpid(  );

  ctrl.binname = mas_strdup( basename( runpath ) );
  pn = strchr( ctrl.binname, '_' );
  if ( pn && *pn++ && *pn )
    ctrl.progname = mas_strdup( pn );


/* ctrl.pkgname=mas_strdup(_pkgname); */
  return r;
}

static int
mas_post_init( void )
{
  int r = 0;

  WMSG( "POST-INIT" );
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
              ( unsigned long ) ctrl.stamp.first_lts, getpid(  ) );
    ctrl.logpath = mas_strdup( opts.logdir );
    ctrl.logpath = mas_strcat_x( ctrl.logpath, namebuf );
    WMSG( "LOG: [%s]", ctrl.logpath );
  }
  else
  {
    EMSG( "logdir not set" );
  }
  if ( !ctrl.is_parent )
  {
    if ( opts.msgfilename )
    {
      HMSG( "MESSAGES to %s", opts.msgfilename );
      MAS_LOG( "(%d) init msg to set file e%d", r, errno );
      IEVAL( r, mas_msg_set_file( opts.msgfilename ) );
      MAS_LOG( "(%d) init msg set file done e%d", r, errno );

      /* TODO if console: */
      MFP( "\x1b[H\x1b[2J" );
    }
    IEVAL( r, mas_init_message(  ) );
  }
  MAS_LOG( "(%d) init done e%d", r, errno );
  WMSG( "(%d) POST INIT DONE", r );
  return r;
}

int
mas_init_restart_count( void )
{
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
mas_init( int argc, char **argv, char **env )
{
  int r = 0;

  HMSG( "INIT" );
  ctrl.stamp.lts = ( unsigned long ) time( NULL );
  ctrl.stamp.first_lts = ctrl.stamp.lts;
  IEVAL( r, mas_init_restart_count(  ) );
  MAS_LOG( "@ %u. init @ %lu -> %lu (%lu)", ctrl.restart_cnt, ( unsigned long ) ctrl.stamp.first_lts, ( unsigned long ) ctrl.stamp.lts,
           ( unsigned long ) ctrl.stamp.prev_lts );
  /* if ( ctrl.is_server ) */

  if ( !( mas_init_argv( argc, argv, env ) > 1 ) )
    IEVAL( r, mas_init_env(  ) );

  /* HMSG( "opts.argv[0]: %s", opts.argv[0] ); */
  /* mas_init_message(  ); */
  /* atexit( atexit_fun ); */
  IEVAL( r, mas_init_sig(  ) );
  WMSG( "(%d) INIT %d", r, __LINE__ );

  IEVAL( r, mas_cli_options( opts.argc, opts.argv ) );
  ctrl.argv_nonoptind = r;
  IEVAL( r, mas_ctrl_init( &opts ) );

  return r;
}

static int
mas_init_vplus( va_list args )
{
  int r = 0;
  int pos = 0;
  typedef int ( *v_t ) ( void );
  v_t fun;

  WMSG( "INIT V+" );
  /* for ( v_t fun = NULL; r >= 0 && !ctrl.is_parent; fun = va_arg( args, v_t ) ) */
  while ( r >= 0 && !ctrl.is_parent && ( fun = va_arg( args, v_t ) ) )
  {
    IEVAL( r, ( fun ) (  ) );
    MAS_LOG( "(%d) init + #%d", r, pos );
    /* ( ctrl.error_handler ) ( FL, 77 ); */
    pos++;
  }
  MAS_LOG( "(%d) init + done", r );
  return r;
}

int
mas_init_uuid( void )
{
#ifdef HAVE_LIBUUID
  if ( !opts.uuid )
  {
    uuid_t uuid;
    char buffer[256];

    memset( uuid, 0, sizeof( uuid ) );
    uuid_generate( uuid );
    uuid_unparse_lower( uuid, buffer );
    opts.uuid = mas_strdup( buffer );
  }
#endif
  return 0;
}

int
mas_init_plus( int argc, char **argv, char **env, ... )
{
  int r = 0;
  va_list args;

  WMSG( "INIT+ %s : %s", ctrl.is_server ? "SERVER" : "CLIENT", !ctrl.is_client ? "SERVER" : "CLIENT" );
  IEVAL( r, mas_pre_init_runpath( argv[0] ) );
  IEVAL( r, mas_pre_init_proc(  ) );
  /* uuid BEFORE opt_files !! */
  IEVAL( r, mas_init_uuid(  ) );
  HMSG( "UUID %s", opts.uuid );
  IEVAL( r, mas_pre_init_opt_files(  ) );
  HMSG( "UUID %s", opts.uuid );
  if ( r < 0 )
  {
    r = 0;
    IEVAL( r, mas_pre_init_default_opts(  ) );
    WMSG( "(%d) PRE-INIT-DEF", r );
  }
  WMSG( "(%d) PRE-INIT", r );
#ifdef MAS_USE_CURSES
  /* // r = mas_init_curses(  ); */
  /* IEVAL( r, mas_init_curses(  ) ); */
#endif
  IEVAL( r, mas_init( argc, argv, env ) );
  {
    va_start( args, env );
    IEVAL( r, mas_init_vplus( args ) );
    va_end( args );
  }
  IEVAL( r, mas_post_init(  ) );
  HMSG( "INIT %s", r < 0 ? "FAIL" : "OK" );
  return r;
}

void
mas_destroy( void )
{
  WMSG( "DESTROY" );
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
      WMSG( "execvp %s %s", opts.argv[0], opts.argv[1] );
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
      IEVAL( r, execvp( opts.argv[0], &opts.argv[0] ) );
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

  if ( ctrl.exename )
    mas_free( ctrl.exename );
  ctrl.exename = NULL;

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

  snprintf( name, sizeof( name ), "MAS_ZOCROMAS_RESTART_%u", ctrl.threads.n.main.pid );
  value = getenv( name );
  if ( ctrl.stderrfile && value )
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
