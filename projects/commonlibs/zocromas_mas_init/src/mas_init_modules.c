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

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/log/mas_log.h>
#include <mastar/log/mas_logger.h>

#ifdef MAS_USE_CURSES
#  include <mastar/msg/mas_curses.h>
#endif

#include <mastar/options/mas_opts.h>
#include <mastar/options/mas_opts_save.h>
#include <mastar/options/mas_opts_restore.h>

#include <mastar/options/mas_cli_opts.h>

/* #include "mas_sig.h" */
#include "mas_init.h"


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

/* Moved to mas_control.c constructor */
/* int                                                                    */
/* mas_init_proc( mas_options_t * popts, const char **message )           */
/* {                                                                      */
/*   EVAL_PREPARE;                                                        */
/*   int r = 0;                                                           */
/*   char lexe[256];                                                      */
/*                                                                        */
/*   (* struct stat sb; *)                                                */
/*                                                                        */
/*   HMSG( "PRE-INIT LINKNAME '%s' / '%s'", ctrl.exepath, ctrl.exename ); */
/*   sprintf( lexe, "/proc/%u/exe", getpid(  ) );                         */
/*   (* if ( lstat( lexe, &sb ) >= 0 ) *)                                 */
/*   (* IEVAL( r, lstat( lexe, &sb ) ); *)                                */
/*   if ( !( r < 0 ) && !( ctrl.exepath && ctrl.exename ) )               */
/*   {                                                                    */
/*     char linkname[512];                                                */
/*                                                                        */
/*     (* size_t sz; *)                                                   */
/*                                                                        */
/*     (* sz = ( sb.st_size ? sb.st_size : 512 ) + 1; *)                  */
/*     (* linkname = mas_malloc( sz ); *)                                 */
/*     (* r = readlink( lexe, linkname, sz ); *)                          */
/*     IEVAL( r, readlink( lexe, linkname, sizeof( linkname ) ) );        */
/*     if ( !( r < 0 ) )                                                  */
/*     {                                                                  */
/*       linkname[r] = '\0';                                              */
/*       HMSG( "(%d) LINKNAME '%s'=>'%s'", r, lexe, linkname );           */
/*     }                                                                  */
/*     {                                                                  */
/*       CTRL_PREPARE;                                                    */
/*       if ( ctrl.exepath )                                              */
/*         mas_free( ctrl.exepath );                                      */
/*       ctrl.exepath = NULL;                                             */
/*       ctrl.exepath = mas_strdup( linkname );                           */
/*       if ( ctrl.exename )                                              */
/*         mas_free( ctrl.exename );                                      */
/*       ctrl.exename = NULL;                                             */
/*       if ( ctrl.exepath )                                              */
/*         ctrl.exename = mas_strdup( basename( ctrl.exepath ) );         */
/*     }                                                                  */
/*   }                                                                    */
/*   if ( message )                                                       */
/*     *message = __func__;                                               */
/*                                                                        */
/*   return r >= 0 ? 0 : r;                                               */
/* }                                                                      */

int
mas_init_message( mas_options_t * popts, const char **message )
{
  if ( popts->has_init_message )
  {
    CTRL_PREPARE;               /* MFP... */
    if ( popts->init_message )
    {
/* #pragma GCC diagnostic push                                */
/*           (* HMSG( popts->init_message ); *)               */
/* (* #pragma message "Compiling " __FILE__ "..." *)          */
/* #pragma GCC diagnostic ignored "-Wformat-security"         */
/* (* #pragma GCC diagnostic ignored "-Wformat-nonliteral" *) */
/* (* #pragma GCC diagnostic ignored "-Wformat" *)            */
/* (* #pragma GCC diagnostic ignored "-Wall" *)               */
/*           MFP( popts->init_message );                      */
/* #pragma GCC diagnostic pop                                 */
      char *esc = NULL;

      esc = mas_escape( popts->init_message );
      if ( esc )
      {
        /* char *d; */

        /* d = mas_dump2( esc, strlen( esc ), 64 ); */
        MFPL( esc );
        /* HMSG( "DUMP ESC:%s", d ); */
        /* mas_free( d ); */
        mas_free( esc );
      }
      /* HMSG( "INIT_MESSAGE [%s]", popts->init_message ); */
    }
    else
    {
      /* (* MFP( "\x1b[H\x1b[2J" ); *) */
      /* (* MFP( "\x1b" "c" ); *)      */
      MFPL( "\x1b" "c" );
    }
  }
  WMSG( "INIT MESSAGE" );

#ifdef MAS_USE_CURSES
  if ( use_curses )
  {
    CTRL_PREPARE;
    extern unsigned long __MAS_LINK_DATE__;
    extern unsigned long __MAS_LINK_TIME__;
    extern unsigned long __MAS_LINK_TIMESTAMP__;

    IMSG( "[%s] %s V.%s built at %s : %lx : %lx : %lu; #%u (%s)", ctrl.progname, PACKAGE_NAME, PACKAGE_VERSION, MAS_C_DATE,
          ( unsigned long ) ( &__MAS_LINK_DATE__ ), ( unsigned long ) ( &__MAS_LINK_TIME__ ), ( unsigned long ) ( &__MAS_LINK_TIMESTAMP__ ),
          ctrl.restart_cnt, ctrl.stamp.vtsc );
    IMSG( "[%s] pid=%u(x%x) ; tid:%u [%lx]", ctrl.progname, ctrl.threads.n.main.pid, ctrl.threads.n.main.pid,
          ( unsigned ) ctrl.threads.n.main.tid, ( unsigned long ) ctrl.threads.n.main.thread );
  }
  else
#endif
  {
    CTRL_PREPARE;
    extern unsigned long __MAS_LINK_DATE__;
    extern unsigned long __MAS_LINK_TIME__;
    extern unsigned long __MAS_LINK_TIMESTAMP__;

    /* IMSG( "\x1b""c" ); */
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
  if ( message )
    *message = __func__;

  return 0;
}

int
mas_init_opt_files( mas_options_t * popts, const char **message )
{
  int r = 0;
  char sppid[64] = "";

  HMSG( "INIT OPT FILES" );
  HMSG( "PPID: %u; BASH VAR: %s", getppid(  ), getenv( "MAS_PID_AT_BASHRC" ) );
  MAS_LOG( "PPID: %u BASH VAR: %s", getppid(  ), getenv( "MAS_PID_AT_BASHRC" ) );
  {
    snprintf( sppid, sizeof( sppid ), "%lu", ( unsigned long ) getppid(  ) );
  }
  if ( *sppid )
  {
    CTRL_PREPARE;
    EVAL_PREPARE;
    int rzero = 0;
    const char *name = ctrl.exename;

    /* const char *name = ctrl.progname; */
    HMSG( "REST.ZERO:%s", name );
    IEVAL_OPT( rzero, mas_opts_restore_zero( popts, name ) );
    {
      int rone = 0;

#ifdef MAS_ONE_OF_CONFIGS

      if ( popts->read_user_opts )
      {
        /* IEVAL_OPT( rone, mas_opts_restore_user_plus( NULL, name, ".", getenv( "MAS_PID_AT_BASHRC" ), NULL ) ); */
        if ( popts->read_user_opts_plus )
        {
          IEVAL_OPT( rone, mas_opts_restore_user_plus( NULL, name, ".", sppid, NULL ) );
        }
        if ( !( rone > 0 ) )
        {
          HMSG( "REST.USER:%s", name );
          IEVALM( r, mas_opts_restore_user( NULL, name ), "(%d) no opt file(s) for prog: '%s'", name );
        }
      }
#else
      int rtwo = 0;

      if ( popts->read_user_opts )
      {
        IEVAL_OPT( rone, mas_opts_restore_user( popts, NULL, name ) );
        if ( popts->read_user_opts_plus )
        {
          /* IEVAL_OPT( rtwo, mas_opts_restore_user_plus( NULL, name, ".", getenv( "MAS_PID_AT_BASHRC" ), NULL ) ); */
          HMSG( "REST.USER +:%s", name );
          IEVAL_OPT( rtwo, mas_opts_restore_user_plus( NULL, name, ".", sppid, NULL ) );
        }
      }
#endif
    }
  }
  /* HMSG( "UUID %s", popts->uuid ); */
  if ( r < 0 )
  {
    EVAL_PREPARE;
    r = 0;
    IEVAL( r, mas_pre_init_default_opts( popts ) );
    WMSG( "(%d) PRE-INIT-DEF", r );
  }
  if ( message )
    *message = __func__;

  return r >= 0 ? 0 : r;
}

int
mas_init_set_msg_file( mas_options_t * popts, const char **message )
{
  CTRL_PREPARE;
  EVAL_PREPARE;
  int r = 0;

  if ( !ctrl.is_parent )
  {
    HMSG( "MESSAGES (%d) to %s", ctrl.messages ? 1 : 0, popts ? popts->msgfilename : NULL );
    if ( popts->msgfilename )
    {
      MAS_LOG( "(%d) init msg to set file e%d", r, errno );
      IEVAL( r, mas_msg_set_file( popts->msgfilename, 0 ) );
      MAS_LOG( "(%d) init msg set file done e%d", r, errno );

      /* TODO if console: */
    }
    /* IEVAL( r, mas_init_message(  ) ); */
  }
  if ( message )
    *message = __func__;

  return r > 0 ? 0 : r;
}

int
mas_init_uuid( mas_options_t * popts, const char **message )
{
#ifdef HAVE_LIBUUID
  if ( !popts->uuid )
  {
    uuid_t uuid;
    char buffer[256];

    memset( uuid, 0, sizeof( uuid ) );
    uuid_generate( uuid );
    uuid_unparse_lower( uuid, buffer );
    popts->uuid = mas_strdup( buffer );
  }
#endif
  HMSG( "UUID %s", popts->uuid );
  if ( message )
    *message = __func__;

  return 0;
}

int
mas_post_init( mas_options_t * popts, const char **message )
{
  int r = 0;

  HMSG( "POST-INIT >" );
#if 0
  if ( r >= 0 && !popts->hosts_num )
  {
    char *defhost = NULL;

    if ( popts->env_hostname )
      defhost = getenv( popts->env_hostname );
    if ( !defhost )
      defhost = "localhost";
    popts->hosts_num = mas_add_argv_arg( popts->hosts_num, &popts->hosts, defhost );

    /* for ( int ih = 0; ih <= popts-> hosts_num; ih++ )                     */
    /* {                                                                  */
    /*   thMSG( "@@@@@@ %d. host %s (%s)", ih, popts-> hosts[ih], defhost ); */
    /* }                                                                  */
  }
#endif
/*
  if ( !popts-> dir.log )
    popts-> dir.log = mas_strdup( ".........." );
*/
  if ( r >= 0 && popts->dir.log )
  {
    CTRL_PREPARE;
    char namebuf[512];

    snprintf( namebuf, sizeof( namebuf ), "/%s.%s.%lu.%u.log", ctrl.is_client ? "client" : "server", ctrl.is_parent ? "parent" : "child",
              ( unsigned long ) ctrl.stamp.first_lts, getpid(  ) );
    ctrl.logpath = mas_strdup( popts->dir.log );
    ctrl.logpath = mas_strcat_x( ctrl.logpath, namebuf );
    WMSG( "LOG: [%s]", ctrl.logpath );
    ctrl.log = popts->log.enable ? 1 : 0;
  }
  else
  {
    EMSG( "dir.log not set" );
  }
  HMSG( "(%d) POST INIT", r );
  if ( message )
    *message = __func__;

  return r >= 0 ? 0 : r;
}

__attribute__ ( ( constructor( 2001 ) ) )
     static void master_constructor( void )
{
  fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno );
}
