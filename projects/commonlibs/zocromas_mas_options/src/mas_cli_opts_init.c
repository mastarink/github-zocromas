#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/control/mas_control.h>
#include <mastar/types/mas_control_types.h>


#include "mas_cli_opts_data.h"

#include "mas_cli_opts_def.h"

#include "mas_cli_opts_init.h"


static char *
mas_cli_enabled_options( mas_options_t * popts )
{
  /* struct option       */
  /* {                   */
  /*   const char *name; */
  /*   int has_arg;      */
  /*   int *flag;        */
  /*   int val;          */
  /* };                  */
  char *s = NULL;

  /* for ( int io = 0; io < sizeof( cli_longopts ) / sizeof( cli_longopts[0] ); io++ ) */
  if ( popts && popts->cli_longopts )
    for ( int io = 0; io < popts->cli_longopts_num; io++ )
    {
      char opt[10];

      if ( popts->cli_longopts[io].val < MAS_CLI_OPT_PLUS )
      {
        opt[0] = ( char ) popts->cli_longopts[io].val;
        opt[1] = 0;
        opt[2] = 0;
        if ( popts->cli_longopts[io].has_arg == required_argument /* || popts->cli_longopts[io].has_arg == optional_argument */  )
          opt[1] = ':';
        s = mas_strcat_x( s, opt );
      }
    }
  return s;
}

static long
mas_cli_optval( const char *arg, long def, int *pr )
{
  long val = 0;

  if ( arg && *arg )
  {
    val = strtol( arg, NULL, 10 );
    if ( ( errno == ERANGE && ( val == LONG_MAX || val == LONG_MIN ) ) || ( errno != 0 && val == 0 ) )
    {
      val = 0;
      *pr = -1;
    }
    else
      *pr = 0;
  }
  else if ( pr )
  {
    val = def;
    *pr = 1;
  }

  return val;
}

static int
mas_cli_make_option( mas_options_t * popts, int opt, const char *m_optarg )
{
  EVAL_PREPARE;
  int r = 0;
  int v = 0;

  mas_optionx_t *optx __attribute__ ( ( unused ) ) = NULL;

  optx = mas_cli_find_optx( opt );
  /* HMSG( "CLI M/O %d [%c] optx:%d", opt, opt > ' ' && opt <= 'z' ? opt : '-', optx ? 1 : 0 ); */
  switch ( opt )
  {
  case MAS_CLI_OPT_TEST:
    HMSG( "O:Test" );
    OPT_SFLAG( popts, test, 1 );
    HMSG( "TEST T %x\n", popts->flag.name.test );
    break;
  case MAS_CLI_OPT_HELP:
    /* for ( int io = 0; io < sizeof( cli_longopts ) / sizeof( cli_longopts[0] ); io++ ) */
    if ( popts )
      for ( int io = 0; io < popts->cli_longopts_num; io++ )
      {
        /* HMSG( "O:%s", cli_longopts[io].name ); */
        HMSG( "O:%s", popts->cli_longopts[io].name );
      }
    break;
  case MAS_CLI_OPT_INFO:
    HMSG( "O:INFO" );
    OPT_SFLAG( popts, info, 1 );
    break;
  case MAS_CLI_OPT_QUIT:
    HMSG( "O:QUIT" );
    OPT_SFLAG( popts, quit, 1 );
    break;
  case MAS_CLI_OPT_COMMAND:
    HMSG( "O:COMMAND %s", m_optarg );
    mas_ctrl_add_command( m_optarg );
    break;
  case MAS_CLI_OPT_MSGTO:
    HMSG( "O:MSG>%s", m_optarg );
    if ( popts->msgfilename )
      mas_free( popts->msgfilename );
    popts->msgfilename = NULL;

    if ( m_optarg && *m_optarg )
      popts->msgfilename = mas_strdup( m_optarg );
    break;
  case MAS_CLI_OPT_STDERRTO:
    HMSG( "O:STDERR>%s", m_optarg );
    if ( popts->stderr_filename )
      mas_free( popts->stderr_filename );
    popts->stderr_filename = NULL;

    if ( m_optarg && *m_optarg )
      popts->stderr_filename = mas_strdup( m_optarg );
    break;
  case MAS_CLI_OPT_STDOUTTO:
    HMSG( "O:MSG>%s", m_optarg );
    if ( popts->stdout_filename )
      mas_free( popts->stdout_filename );
    popts->stdout_filename = NULL;

    if ( m_optarg && *m_optarg )
      popts->stdout_filename = mas_strdup( m_optarg );
    break;
  case MAS_CLI_OPT_HOST:
    popts->hostsv.c = mas_add_argv_arg( popts->hostsv.c, &popts->hostsv.v, m_optarg );
    mMSG( "HOST %d: %s [%p]", popts->hostsv.c, m_optarg, ( void * ) popts->hostsv.v );
    break;
  case MAS_CLI_OPT_PROTO:
    popts->protosv.c = mas_add_argv_arg( popts->protosv.c, &popts->protosv.v, m_optarg );
    mMSG( "PROTO %d: %s [%p]", popts->protosv.c, m_optarg, ( void * ) popts->protosv.v );
    break;
  case MAS_CLI_OPT_PORT:
    if ( m_optarg && *m_optarg )
    {
      popts->default_port = 0;

      sscanf( m_optarg, "%u", &popts->default_port );
      /* HMSG( "O:PORT: %u", popts-> default_port ); */
    }
    break;
  case MAS_CLI_OPT_MODSDIR:
    if ( popts->dir.mods )
      mas_free( popts->dir.mods );
    popts->dir.mods = NULL;

    if ( m_optarg && *m_optarg )
      popts->dir.mods = mas_strdup( m_optarg );

    break;
  case MAS_CLI_OPT_PROTODIR:
    if ( popts->dir.proto )
      mas_free( popts->dir.proto );
    popts->dir.proto = NULL;

    if ( m_optarg && *m_optarg )
      popts->dir.proto = mas_strdup( m_optarg );

    break;
  case MAS_CLI_OPT_LOGDIR:
    if ( popts->dir.log )
      mas_free( popts->dir.log );
    popts->dir.log = NULL;

    if ( m_optarg && *m_optarg )
      popts->dir.log = mas_strdup( m_optarg );

    break;
  case MAS_CLI_OPT_HISTORYDIR:
    if ( popts->dir.history )
      mas_free( popts->dir.history );
    popts->dir.history = NULL;

    if ( m_optarg && *m_optarg )
      popts->dir.history = mas_strdup( m_optarg );

    break;
  case MAS_CLI_OPT_LISTENER_SINGLE:
    OPT_SFLAG( popts, listener_single, 1 );
    break;
  case MAS_CLI_OPT_TRANSACTION_SINGLE:
    OPT_SFLAG( popts, transaction_single, 1 );
    break;
  case MAS_CLI_OPT_NOSYSDAEMON:
    OPT_SFLAG( popts, daemon.sys, 0 );
    break;
  case MAS_CLI_OPT_SYSDAEMON:
    OPT_SFLAG( popts, daemon.sys, 1 );
    break;
  case MAS_CLI_OPT_NODAEMON:
    OPT_SFLAG( popts, daemon.disable, 1 );
    break;
  case MAS_CLI_OPT_NONODAEMON:
    OPT_SFLAG( popts, daemon.disable, 0 );
    break;
  case MAS_CLI_OPT_NOSAVE_USER_OPTS:
    OPT_SFLAG( popts, save_user_opts, 0 );
    break;
  case MAS_CLI_OPT_SAVE_USER_OPTS:
    if ( popts->save_user_opts_filename )
      mas_free( popts->save_user_opts_filename );
    popts->save_user_opts_filename = NULL;
    if ( m_optarg && *m_optarg )
      popts->save_user_opts_filename = mas_strdup( m_optarg );
    OPT_SFLAG( popts, save_user_opts, 1 );
    break;
  case MAS_CLI_OPT_NOSINGLE_CHILD:
    OPT_SFLAG( popts, single_child, 0 );
    break;
  case MAS_CLI_OPT_SINGLE_CHILD:
    OPT_SFLAG( popts, single_child, 1 );
    break;
  case MAS_CLI_OPT_NOSINGLE_INSTANCE:
    OPT_SFLAG( popts, single_instance, 0 );
    break;
  case MAS_CLI_OPT_SINGLE_INSTANCE:
    OPT_SFLAG( popts, single_instance, 1 );
    break;
  case MAS_CLI_OPT_NOLOGGER:
    OPT_SFLAG( popts, log.run, 0 );
    break;
  case MAS_CLI_OPT_LOGGER:
    OPT_SFLAG( popts, log.run, 1 );
    break;
  case MAS_CLI_OPT_NOLOG:
    OPT_SFLAG( popts, log.enable, 0 );
    break;
  case MAS_CLI_OPT_LOG:
    OPT_SFLAG( popts, log.enable, 1 );
    break;
  case MAS_CLI_OPT_NOTICKER:
    OPT_SFLAG( popts, noticker, 1 );
    break;
  case MAS_CLI_OPT_TICKER:
    OPT_SFLAG( popts, noticker, 0 );
    break;
  case MAS_CLI_OPT_TICKER_MODE:
    if ( m_optarg && *m_optarg )
    {
      popts->ticker_mode = 0;

      sscanf( m_optarg, "%u", &popts->ticker_mode );
    }
    break;
  case MAS_CLI_OPT_NOREAD_USER_OPTS:
    OPT_SFLAG( popts, read_user_opts, 0 );
    break;
  case MAS_CLI_OPT_READ_USER_OPTS:
    OPT_SFLAG( popts, read_user_opts, 1 );
    break;
  case MAS_CLI_OPT_NOREAD_USER_OPTS_PLUS:
    OPT_SFLAG( popts, read_user_opts_plus, 0 );
    break;
  case MAS_CLI_OPT_READ_USER_OPTS_PLUS:
    OPT_SFLAG( popts, read_user_opts_plus, 1 );
    break;

  case MAS_CLI_OPT_NOOVERWRITE_USER_OPTS:
    OPT_SFLAG( popts, overwrite_user_opts, 0 );
    break;
  case MAS_CLI_OPT_OVERWRITE_USER_OPTS:
    OPT_SFLAG( popts, overwrite_user_opts, 1 );
    break;
  case MAS_CLI_OPT_NOOVERWRITE_USER_OPTS_PLUS:
    OPT_SFLAG( popts, overwrite_user_opts_plus, 0 );
    break;
  case MAS_CLI_OPT_OVERWRITE_USER_OPTS_PLUS:
    OPT_SFLAG( popts, overwrite_user_opts_plus, 1 );
    break;


  case MAS_CLI_OPT_NOWATCHER:
    OPT_SFLAG( popts, nowatcher, 1 );
    break;
  case MAS_CLI_OPT_NONOWATCHER:
    OPT_SFLAG( popts, nowatcher, 0 );
    break;
  case MAS_CLI_OPT_NOLISTENER:
    popts->nolistener = mas_cli_optval( m_optarg, 30, &v );
    break;
  case MAS_CLI_OPT_LISTENER:
    popts->nolistener = 0;
    break;
  case MAS_CLI_OPT_NOPIDFILE:
    OPT_SFLAG( popts, nopidfile, 1 );
    break;
  case MAS_CLI_OPT_PIDFILE:
    OPT_SFLAG( popts, nopidfile, 0 );
    break;
  case MAS_CLI_OPT_NOLISTEN:
    popts->nolisten = mas_cli_optval( m_optarg, 30, &v );
    break;
  case MAS_CLI_OPT_LISTEN:
    popts->nolisten = 0;
    break;
  case MAS_CLI_OPT_NOMASTER:
    popts->nomaster = mas_cli_optval( m_optarg, 30, &v );
    break;
  case MAS_CLI_OPT_MASTER_THREAD:
    OPT_SFLAG( popts, make_master_thread, 1 );
    break;
  case MAS_CLI_OPT_NOMASTER_THREAD:
    OPT_SFLAG( popts, make_master_thread, 0 );
    break;
  case MAS_CLI_OPT_MASTER:
    popts->nomaster = 0;
    break;
  case MAS_CLI_OPT_NOREDIRECT_STD:
    OPT_SFLAG( popts, daemon.disable_redirect_std, 1 );
    break;
  case MAS_CLI_OPT_REDIRECT_STD:
    OPT_SFLAG( popts, daemon.disable_redirect_std, 0 );
    break;
  case MAS_CLI_OPT_NOCLOSE_STD:
    OPT_SFLAG( popts, daemon.disable_close_std, 1 );
    break;
  case MAS_CLI_OPT_CLOSE_STD:
    OPT_SFLAG( popts, daemon.disable_close_std, 0 );
    break;
  case MAS_CLI_OPT_NOMESSAGES_PARENT:
    OPT_SFLAG( popts, nomessages_parent, 1 );
    break;
  case MAS_CLI_OPT_MESSAGES_PARENT:
    OPT_SFLAG( popts, nomessages_parent, 0 );
    break;
  case MAS_CLI_OPT_NOMESSAGES_CHILD:
    OPT_SFLAG( popts, nomessages_child, 1 );
    break;
  case MAS_CLI_OPT_MESSAGES_CHILD:
    OPT_SFLAG( popts, nomessages_child, 0 );
    break;
  case MAS_CLI_OPT_NOMESSAGES:
    OPT_SFLAG( popts, nomessages, 1 );
    break;
  case MAS_CLI_OPT_MESSAGES:
    OPT_SFLAG( popts, nomessages, 0 );
    break;
  case MAS_CLI_OPT_NOMSG:
    /* HMSG( "O:flags: %lo", popts-> msg_flag.bits ); */
    popts->flag.name.msg.bits = 0;
    /* HMSG( "O:flags: %lo", popts-> msg_flag.bits ); */

    /* popts->msg_flag.bit.msg_trace = 0; */
    OPT_SFLAG( popts, msg.name.msg_trace, 0 );

    /* HMSG( "O:flags: %lo", popts-> msg_flag.bits ); */
    break;
  case MAS_CLI_OPT_MSG:
    if ( 0 == strcmp( "mem", m_optarg ) )
    {
      /* popts->msg_flag.bit.msg_mem = 1; */
      OPT_SFLAG( popts, msg.name.msg_mem, 1 );
    }
    break;
  case MAS_CLI_OPT_INIT_MSG:
    /* OPT_SFLAG( popts, has_init_message, 1 ); */
    if ( popts->init_message )
      mas_free( popts->init_message );
    popts->init_message = NULL;

    if ( m_optarg && *m_optarg )
      popts->init_message = mas_strdup( m_optarg );

    break;
  case MAS_CLI_OPT_NOHOSTS:
    popts->hostsv.c = mas_del_argv( popts->hostsv.c, popts->hostsv.v, 0 );
    popts->hostsv.v = NULL;

    break;
  case MAS_CLI_OPT_NOPROTOS:
    popts->protosv.c = mas_del_argv( popts->protosv.c, popts->protosv.v, 0 );
    popts->protosv.v = NULL;

    break;
  case MAS_CLI_OPT_EXITSLEEP:
    popts->exitsleep = ( unsigned ) ( m_optarg && *m_optarg ? strtol( m_optarg, NULL, 10 ) : 30 );
    break;

  default:                     /* '?' ; ':' */
    {
      CTRL_PREPARE;
      /* fprintf( ctrl.stderrfile, "Usage: %s [-t nsecs] [-n] name\n", oargv[0] ); */

      /* ctrl.in_client = 0; */
      ctrl.fatal = 1;
      ctrl.keep_listening = 0;
      HMSG( "O:CLI unknown opt:%d [%c] optind:%d : errno:%d", opt, opt > ' ' && opt <= 'z' ? opt : '-', optind, errno );
      IEVAL( r, -88 );
    }
    break;
  }
  if ( v < 0 )
  {
    HMSG( "O:CLI wrong value '%s'", m_optarg );
    IEVAL( r, -1 );
  }
  /* HMSG( "O:getopt_long:%d Usage: %s [--help -h]", oargv[0] ); */
  return r;
}

static int
_mas_cli_options( mas_options_t * popts, int oargc, char *const *oargv )
{
  CTRL_PREPARE;
  int r = 0;
  int opt = 0;
  int indx = 0;
  int afterlast = 0;
  char *enabled_opts __attribute__ ( ( unused ) ) = NULL;

  optind = 1;
  optarg = NULL;
  /* opterr = 0; */
  optopt = 0;
  indx = 0;
  enabled_opts = mas_cli_enabled_options( popts );
  HMSG( "CLI %d", ( popts && popts->cli_longopts ) ? 1 : 0 );
  extern struct option mas_cli_longopts_ctable[];

  if ( popts && popts->cli_longopts )
    while ( r >= 0 && !ctrl.fatal
            && ( opt = getopt_long( oargc, oargv, enabled_opts, mas_cli_longopts_ctable /*popts->cli_longopts */ , &indx ) ) >= 0 )
    {
      /* HMSG( "CLI opt:%d: optind:%d err:%d / %d indx:%d.%s", opt, optind, opt == '?', opt == ':', indx, popts->cli_longopts[indx].name ); */
      IEVAL( r, mas_cli_make_option( popts, opt, optarg ) );
      afterlast = optind;
      /* HMSG( "(%d) CLI %d: %d", r, opt, optind ); */
    }
  MAS_LOG( "(%d) cli options made", r );
  /* HMSG( "CLI r=%d; opt=%d", r, opt ); */
  mas_free( enabled_opts );
  return r < 0 ? r : afterlast;
}

int
mas_cli_options_init( mas_options_t * popts, const char **message )
{
  int r = -1;
  int oargc;
  char *const *oargv;

  if ( popts )
  {
    CTRL_PREPARE;
    oargc = popts->argvv.c;
    oargv = popts->argvv.v;
    r = _mas_cli_options( popts, oargc, oargv );
    ctrl.argv_nonoptind = r;
  }
  if ( message )
    *message = __func__;

  HMSG( "(%d) INIT CLI", r );
  return r >= 0 ? 0 : r;
}

int
mas_cli_options_argv_init( mas_options_t * popts, int oargc, char **oargv, char **env )
{
  CTRL_PREPARE;
  ctrl.launchervv.v = oargv;
  ctrl.launchervv.c = oargc;
  ctrl.launcherev.v = env;
  /* if ( oargc > 1 ) */
  {
    for ( int ia = 0; ia < oargc; ia++ )
    {
      popts->argvv.c = mas_add_argv_arg( popts->argvv.c, &popts->argvv.v, oargv[ia] );
    }
    /* for ( int ia = 0; ia < oargc; ia++ )                              */
    /* {                                                                */
    /*   mMSG( "@: %d of %d. arg:'%s'", ia, popts-> argvv.c, popts-> argvv.v[ia] ); */
    /* }                                                                */
  }
  HMSG( "INIT ARGV %d", popts->argvv.c );
  return popts->argvv.c;
}

void
mas_cli_options_argv_destroy( mas_options_t * popts )
{
  mas_del_argv( popts->argvv.c, popts->argvv.v, 0 );
  popts->argvv.c = 0;
  popts->argvv.v = NULL;
}

void
mas_cli_options_destroy( mas_options_t * popts )
{
  mas_cli_options_argv_destroy( popts );
}
