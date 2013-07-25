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

#ifdef MAS_USE_CURSES
#  include <mastar/msg/mas_curses.h>
#endif

#include <mastar/options/mas_opts.h>
#include <mastar/options/mas_opts_save.h>
#include <mastar/options/mas_opts_restore.h>

#include <mastar/options/mas_cli_opts.h>

#include "mas_sig.h"
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

int
mas_init_message( mas_options_t * popts, const char **message )
{
  extern mas_control_t ctrl;
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

int
mas_init_proc( mas_options_t * popts, const char **message )
{
  int r = 0;
  char lexe[256];
  struct stat sb;
  char *linkname;

  HMSG( "PRE-INIT" );
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
    {
      extern mas_control_t ctrl;

      ctrl.exepath = linkname;
      ctrl.exename = mas_strdup( basename( ctrl.exepath ) );
    }
  }
  if ( message )
    *message = __func__;

  return r >= 0 ? 0 : r;
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
    extern mas_control_t ctrl;
    int rzero = 0;
    const char *name = ctrl.exename;

    /* const char *name = ctrl.progname; */

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
          IEVAL_OPT( rtwo, mas_opts_restore_user_plus( NULL, name, ".", sppid, NULL ) );
        }
      }
#endif
    }
  }
  /* HMSG( "UUID %s", popts->uuid ); */
  if ( r < 0 )
  {
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
  extern mas_control_t ctrl;
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
      MFP( "\x1b[H\x1b[2J" );
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
  extern mas_control_t ctrl;
  int r = 0;

  EMSG( "POST-INIT" );
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
    char namebuf[512];

    snprintf( namebuf, sizeof( namebuf ), "/%s.%s.%lu.%u.log", ctrl.is_client ? "client" : "server", ctrl.is_parent ? "parent" : "child",
              ( unsigned long ) ctrl.stamp.first_lts, getpid(  ) );
    ctrl.logpath = mas_strdup( popts->dir.log );
    ctrl.logpath = mas_strcat_x( ctrl.logpath, namebuf );
    WMSG( "LOG: [%s]", ctrl.logpath );
    ctrl.log = popts->nolog ? 0 : 1;
  }
  else
  {
    EMSG( "dir.log not set" );
  }
  WMSG( "(%d) POST INIT DONE", r );
  HMSG( "(%d) postINIT done", r );

  if ( message )
    *message = __func__;

  return r >= 0 ? 0 : r;
}
