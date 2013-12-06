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

#include <mastar/options/mas_opts.h>
#include <mastar/options/mas_opts_save.h>
/* #include <mastar/options/mas_opts_restore.h> */
/* #include <mastar/cliopts/mas_cliopts.h> */

#include <mastar/cliopts/mas_cliopts_data.h>
#include <mastar/cliopts/mas_cliopts_init.h>

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

/* static int                                                                             */
/* mas_argv_init( mas_options_t * popts, int argc, char **argv, char **env )              */
/* {                                                                                      */
/*   CTRL_PREPARE;                                                                        */
/*   ctrl.launchervv.v = argv;                                                            */
/*   ctrl.launchervv.c = argc;                                                            */
/*   ctrl.launcherev.v = env;                                                             */
/*   (* if ( argc > 1 ) *)                                                                */
/*   {                                                                                    */
/*     for ( int ia = 0; ia < argc; ia++ )                                                */
/*     {                                                                                  */
/*       popts->argvv.c = mas_add_argv_arg( popts->argvv.c, &popts->argvv.v, argv[ia] );  */
/*     }                                                                                  */
/*     (* for ( int ia = 0; ia < argc; ia++ )                              *)             */
/*     (* {                                                                *)             */
/*     (*   mMSG( "@: %d of %d. arg:'%s'", ia, popts-> argvv.c, popts-> argvv.v[ia] ); *) */
/*     (* }                                                                *)             */
/*   }                                                                                    */
/*   HMSG( "INIT ARGV %d", popts->argvv.c );                                              */
/*   return popts->argvv.c;                                                               */
/* }                                                                                      */

int
mas_env_init( mas_options_t * popts )
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
mas_setup_runpath( char *runpath )
{
  CTRL_PREPARE;
  int r = 0;
  const char *pn;

  ctrl.c.status = MAS_STATUS_START;
  ctrl.error_handler = error_handler_at_init;

  ctrl.start_time = mas_double_time(  );
  ctrl.stamp.start_time = ( unsigned long ) time( NULL );
  /* ctrl.stamp.lts = ( unsigned long ) time( NULL ); */
  ctrl.stamp.first_lts = 0;
  ctrl.c.status = MAS_STATUS_INIT;


  ctrl.threads.n.main.tid = mas_gettid(  );
  ctrl.threads.n.main.pid = getpid(  );
  ctrl.threads.n.main.thread = mas_pthread_self(  );
  ctrl.pserver_thread = &ctrl.threads.n.main;


  ctrl.argvname = mas_strdup( basename( runpath ) );
  pn = strchr( ctrl.argvname, '_' );
  if ( pn && *pn++ && *pn )
    ctrl.progname = mas_strdup( pn );

/* ctrl.pkgname=mas_strdup(_pkgname); */
  return r;
}



int
mas_init( mas_options_t * popts, int argc, char **argv, char **env )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
  int r = 0;

  HMSG( "INIT" );
  IEVAL( r, mas_setup_runpath( argv[0] ) );
  WMSG( "(%d) PRE-INIT", r );
#ifdef MAS_USE_CURSES
  /* // r = mas_curses_init(  ); */
  /* IEVAL( r, mas_curses_init(  ) ); */
#endif

  mas_ctrl_setup( argc, argv, env );

  MAS_LOG( "@ %u. init @ %lu -> %lu (%lu)", ctrl.restart_cnt, ( unsigned long ) ctrl.stamp.first_lts, ( unsigned long ) ctrl.stamp.lts,
           ( unsigned long ) ctrl.stamp.prev_lts );

  /* if ( !( mas_cli_options_argv_init( popts, argc, argv, env ) > 1 ) ) */
  /*   IEVAL( r, mas_env_init( popts ) );                                */
  /* *argv[0]='Z'; */

  /* HMSG( "popts-> argvv.v[0]: %s", popts-> argvv.v[0] ); */
  /* mas_message_init(  ); */
  /* atexit( atexit_fun ); */
  /* IEVAL( r, mas_sig_init(  ) ); */

  /* IEVAL( r, mas_cli_options_init( popts, NULL ) ); */
  /* IEVAL( r, mas_ctrl_init( popts, NULL ) );        */

  return r;
}

static int
mas_init_vplus( mas_options_t * popts, va_list args )
{
  CTRL_PREPARE;
  int r = 0;
  int ifu = 0;
  mas_init_fun_t fun;

  WMSG( "INIT V+" );
  /* for ( mas_init_fun_t fun = NULL; r >= 0 && !ctrl.is_parent; fun = va_arg( args, mas_init_fun_t ) ) */
  while ( !r && !ctrl.is_parent && ( fun = va_arg( args, mas_init_fun_t ) ) && !ctrl.is_parent )
  {
    EVAL_PREPARE;
    const char *msg = NULL;

    HMSG( "(%d) + INIT V #%d", r, ifu );
    IEVAL( r, ( fun ) ( popts, &msg, 0 ) );
    MAS_LOG( "(%d) init + #%d - %s", r, ifu, msg ? msg : "-" );
    HMSG( "(%d) - INIT V #%d %s", r, ifu, msg ? msg : "-" );
    /* ( ctrl.error_handler ) ( FL, 77 ); */
    ifu++;
  }
  HMSG( "(%d) INIT V done (#%d) is_parent:%d", r, ifu, ctrl.is_parent );
  MAS_LOG( "(%d) init + done", r );
  return r;
}

int
mas_init_set_n( mas_options_t * popts, /* int argc, char **argv, char **env, */ int funcnt, mas_init_fun_t * init_funcs )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
  int r = 0;
  int ifu = 0;

  /* WMSG( "INIT+ %s : %s", ctrl.is_server ? "SERVER" : "CLIENT", !ctrl.is_client ? "SERVER" : "CLIENT" ); */
  /* IEVAL( r, mas_init( popts, argc, argv, env ) );                                                       */

  WMSG( "INIT S+" );
  for ( ifu = 0; ifu < funcnt && !r && !ctrl.is_parent && !ctrl.is_parent; ifu++ )
  {
    mas_init_fun_t fun;

    fun = init_funcs[ifu];
    if ( fun )
    {
      const char *msg = NULL;

      HMSG( "(%d) + INIT S #%d [%p] %d:[%p]", r, ifu, ( void * ) ( unsigned long long ) fun, ifu,
            ( void * ) ( unsigned long long ) init_funcs[ifu] );
      IEVAL( r, ( fun ) ( popts, &msg, 0 ) );
      MAS_LOG( "(%d) init + #%d - %s", r, ifu, msg ? msg : "-" );
      HMSG( "(%d) - INIT S #%d %s", r, ifu, msg ? msg : "-" );
      /* ( ctrl.error_handler ) ( FL, 77 ); */
    }
  }
  HMSG( "(%d) INIT V done (#%d) is_parent:%d", r, ifu, ctrl.is_parent );
  MAS_LOG( "(%d) init + done", r );

  MAS_LOG( "(%d) init done e%d", r, errno );
  HMSG( "(%d)INIT %s", r, r < 0 ? "FAIL" : "OK" );
  ctrl.inited = 1;
  return r;
}

int
mas_init_set_z( mas_options_t * popts, /* int argc, char **argv, char **env, */ mas_init_fun_t * init_funcs )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
  int r = 0;
  int ifu = 0;
  mas_init_fun_t *funcs = init_funcs;

  mas_control_t *this = &ctrl;

  MSTAGE( INIT );

  /* WMSG( "INIT+ %s : %s", ctrl.is_server ? "SERVER" : "CLIENT", !ctrl.is_client ? "SERVER" : "CLIENT" ); */
  /* IEVAL( r, mas_init( popts, argc, argv, env ) );                                                       */

  HMSG( "INIT SET+" );
  {
    mas_init_fun_t fun;

    while ( ( fun = *funcs++ ) && !r && !ctrl.is_parent && !ctrl.is_parent )
    {
      const char *msg = NULL;

      HMSG( "(%d) + INIT SET #%d [%p] %d:[%p]", r, ifu, ( void * ) ( unsigned long long ) fun, ifu,
            ( void * ) ( unsigned long long ) init_funcs[ifu] );
      IEVAL( r, ( fun ) ( popts, &msg, 0 ) );
      MAS_LOG( "(%d) init + #%d - %s", r, ifu, msg ? msg : "-" );
      HMSG( "(%d) - INIT SET #%d %s", r, ifu, msg ? msg : "-" );
      /* ( ctrl.error_handler ) ( FL, 77 ); */
      ifu++;
    }
  }
  HMSG( "(%d) INIT V done (#%d) is_parent:%d", r, ifu, ctrl.is_parent );
  MAS_LOG( "(%d) init + done", r );

  MAS_LOG( "(%d) init done e%d", r, errno );
  HMSG( "(%d)INIT %s", r, r < 0 ? "FAIL" : "OK" );
  ctrl.inited = 1;
  return r;
}

int
mas_init_plus( mas_options_t * popts, int argc, char **argv, char **env, ... )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
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
  int r = 0;

  CTRL_PREPARE;
  EVAL_PREPARE;
  char sppid[64] = "";


  snprintf( sppid, sizeof( sppid ), "%lu", ( unsigned long ) getppid(  ) );
  if ( *sppid )
  {
    if ( !ctrl.opts_saved )
      IEVAL( r,
             mas_opts_save_user( popts, NULL,
                                 popts->save_user_opts_filename ? popts->save_user_opts_filename : ( ctrl.progname ? ctrl.progname :
                                                                                                     "Unknown" ) ) );
    HMSG( "(%d)SAVE USER", r );
    r = 0;
    if ( !ctrl.opts_saved_plus )
      IEVAL( r, mas_opts_save_user_plus( popts, NULL, ctrl.progname ? ctrl.progname : "Unknown", ".", sppid, NULL ) );
    HMSG( "(%d)SAVE", r );
    r = 0;
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
  mas_cliopts_argv_destroy( popts );
  mas_cliopts_data_destroy( popts );

  IEVAL( r, mas_opts_delete( popts ) );
  popts = NULL;
  r = 0;

  ctrl.log_disabled = 1;

  IEVAL( r, mas_ctrl_destroy(  ) );
  r = 0;

  MAS_LOG( "destroy done" );
  IEVAL( r, mas_logger_delete( 1 ) );
  r = 0;
  HMSG( "DESTROY DONE" );
#ifdef MAS_TRACEMEM
  {
    /* int k; */
    extern unsigned long memory_balance;

    /* k = print_memlist( ctrl.msgfile, FL ); */
    if ( ctrl.dead_beaf )
    {
      char *bad_mem;

      bad_mem = mas_malloc( 3735928495 );
      if ( bad_mem )
        strcpy( bad_mem, "DEAD BEAF" );
    }
    FMSG( "destroy, memory_balance:%ld (%lX);", memory_balance, memory_balance );
    if ( 0 )
      print_memlist_any( FL, 4, ctrl.stderrfile, ctrl.old_stderrfile, ctrl.msgfile, stderr );
    fflush( ctrl.msgfile );
  }
#endif
}

__attribute__ ( ( constructor( 2100 ) ) )
     static void mas_constructor( void )
{
  CTRL_PREPARE;
  /* char name[512]; */

  /* char *value = NULL; */

  /* fprintf( stderr, "******************** CONSTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_constructor( IL, 0 );

  if ( !ctrl.stderrfile )
    ctrl.stderrfile = stderr;

  /* snprintf( name, sizeof( name ), "MAS_ZOCROMAS_RESTART_%u", ctrl.threads.n.main.pid ); */
  /* value = getenv( name ); */
  /* fprintf( stderr, "******************** [%s='%s'] /CONSTRUCTOr %s e%d\n", name, value, __FILE__, errno ); */
  /* ctrl.is_server = 0; */
  /* ctrl.is_client = 0; */
}

__attribute__ ( ( destructor ) )
     static void mas_destructor( void )
{
  /* fprintf( stderr, "******************** DESTRUCTOr %s e%d\n", __FILE__, errno ); */
  mas_common_destructor( IL, 1 );
}
