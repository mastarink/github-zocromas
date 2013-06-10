#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/control/mas_control.h>


#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;



#include "mas_opts.h"
#include "mas_cli_options.h"


/*
this:
  mas_cli_options.c
related:
  mas_cli_options.h
  mas_opts_types.h
  mas_opts_data.c
  mas_msg_tools.c
  mas_control.c

*/


/* int                                                                                                                        */
/* mas_getopt_long( int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex )      */
/* {                                                                                                                          */
/*   return getopt_long( argc, argv, optstring, longopts, longindex );                                                        */
/* }                                                                                                                          */
/*                                                                                                                            */
/* int                                                                                                                        */
/* mas_getopt_long_only( int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex ) */
/* {                                                                                                                          */
/*   return getopt_long_only( argc, argv, optstring, longopts, longindex );                                                   */
/* }                                                                                                                          */

typedef enum mas_cli_opts_e
{
  MAS_CLI_OPT_NONE,
  MAS_CLI_OPT_TEST = 't',
  MAS_CLI_OPT_HELP = 'h',
  MAS_CLI_OPT_DAEMON = 'd',
  MAS_CLI_OPT_HOST = 'H',
  MAS_CLI_OPT_PORT = 'P',
  MAS_CLI_OPT_MODSDIR = 'M',
  MAS_CLI_OPT_LOGDIR = 'L',
  MAS_CLI_OPT_PLUS = 10000,
  MAS_CLI_OPT_HISTORYDIR,
  MAS_CLI_OPT_PROTODIR,
  MAS_CLI_OPT_PROTO,
  MAS_CLI_OPT_NODAEMON,
  MAS_CLI_OPT_MSG,
  MAS_CLI_OPT_NOMSG,
  MAS_CLI_OPT_READ_HOME_OPTS,
  MAS_CLI_OPT_NOREAD_HOME_OPTS,
  MAS_CLI_OPT_NOHOSTS,
  MAS_CLI_OPT_REDIRECT_STD,
  MAS_CLI_OPT_NOREDIRECT_STD,
  MAS_CLI_OPT_CLOSE_STD,
  MAS_CLI_OPT_NOCLOSE_STD,
  MAS_CLI_OPT_MESSAGES,
  MAS_CLI_OPT_NOMESSAGES,
  MAS_CLI_OPT_NOLOGGER,
  MAS_CLI_OPT_LOGGER,
  MAS_CLI_OPT_NOLOG,
  MAS_CLI_OPT_LOG,
  MAS_CLI_OPT_NOTICKER,
  MAS_CLI_OPT_TICKER,
  MAS_CLI_OPT_TICKER_MODE,
  MAS_CLI_OPT_NOWATCHER,
  MAS_CLI_OPT_WATCHER,
  MAS_CLI_OPT_NOMASTER,
  MAS_CLI_OPT_MASTER,
  MAS_CLI_OPT_NOMASTER_THREAD,
  MAS_CLI_OPT_MASTER_THREAD,
  MAS_CLI_OPT_LISTEN,
  MAS_CLI_OPT_NOLISTEN,
  MAS_CLI_OPT_LISTENER,
  MAS_CLI_OPT_NOLISTENER,
  MAS_CLI_OPT_EXITSLEEP,
  MAS_CLI_OPT_LISTENER_SINGLE,
  MAS_CLI_OPT_TRANSACTION_SINGLE,
  MAS_CLI_OPT_COMMAND,
  MAS_CLI_OPT_MSGTO,
  MAS_CLI_OPT_SINGLE_INSTANCE,
  MAS_CLI_OPT_NOSINGLE_INSTANCE,
  MAS_CLI_OPT_SINGLE_CHILD,
  MAS_CLI_OPT_NOSINGLE_CHILD,
} mas_cli_opts_t;

static char cli_enabled_options[] = "hH:P:L:t";

static struct option cli_longopts[] = {
  {"test", no_argument, NULL, MAS_CLI_OPT_TEST},
  {"help", no_argument, NULL, MAS_CLI_OPT_HELP},
  {"exitsleep", optional_argument, NULL, MAS_CLI_OPT_EXITSLEEP},

  {"single", no_argument, NULL, MAS_CLI_OPT_SINGLE_INSTANCE},
  {"single-instance", no_argument, NULL, MAS_CLI_OPT_SINGLE_INSTANCE},
  {"multi-instance", no_argument, NULL, MAS_CLI_OPT_NOSINGLE_INSTANCE},

  {"single-child", no_argument, NULL, MAS_CLI_OPT_SINGLE_CHILD},
  {"multi-child", no_argument, NULL, MAS_CLI_OPT_NOSINGLE_CHILD},

  {"command", required_argument, NULL, MAS_CLI_OPT_COMMAND},
  {"redirect-messages", required_argument, NULL, MAS_CLI_OPT_MSGTO},
  {"listener-single", no_argument, NULL, MAS_CLI_OPT_LISTENER_SINGLE},
  {"transaction-single", no_argument, NULL, MAS_CLI_OPT_TRANSACTION_SINGLE},

  {"messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES},
  {"nomessages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES},

  {"noredirect-std", no_argument, NULL, MAS_CLI_OPT_NOREDIRECT_STD},
  {"redirect-std", no_argument, NULL, MAS_CLI_OPT_REDIRECT_STD},

  {"noclose-std", no_argument, NULL, MAS_CLI_OPT_NOCLOSE_STD},
  {"close-std", no_argument, NULL, MAS_CLI_OPT_CLOSE_STD},

  {"nologger", no_argument, NULL, MAS_CLI_OPT_NOLOGGER},
  {"logger", no_argument, NULL, MAS_CLI_OPT_LOGGER},

  {"nolog", no_argument, NULL, MAS_CLI_OPT_NOLOG},
  {"log", no_argument, NULL, MAS_CLI_OPT_LOG},

  {"modsdir", required_argument, NULL, MAS_CLI_OPT_MODSDIR},
  {"protodir", required_argument, NULL, MAS_CLI_OPT_PROTODIR},
  {"logdir", required_argument, NULL, MAS_CLI_OPT_LOGDIR},
  {"historydir", required_argument, NULL, MAS_CLI_OPT_HISTORYDIR},

  {"noread-home-config", no_argument, NULL, MAS_CLI_OPT_NOREAD_HOME_OPTS},
  {"read-home-config", no_argument, NULL, MAS_CLI_OPT_READ_HOME_OPTS},

  {"nowatcher", no_argument, NULL, MAS_CLI_OPT_NOWATCHER},
  {"watcher", no_argument, NULL, MAS_CLI_OPT_WATCHER},

  {"noticker", no_argument, NULL, MAS_CLI_OPT_NOTICKER},
  {"ticker", no_argument, NULL, MAS_CLI_OPT_TICKER},
  {"set-ticker-mode", required_argument, NULL, MAS_CLI_OPT_TICKER_MODE},

  {"nomaster", required_argument, NULL, MAS_CLI_OPT_NOMASTER},
  {"master", no_argument, NULL, MAS_CLI_OPT_MASTER},

  {"nomthread", no_argument, NULL, MAS_CLI_OPT_NOMASTER_THREAD},
  {"mthread", no_argument, NULL, MAS_CLI_OPT_MASTER_THREAD},

  {"nolistener", required_argument, NULL, MAS_CLI_OPT_NOLISTENER},
  {"listener", no_argument, NULL, MAS_CLI_OPT_LISTENER},

  {"nolisten", required_argument, NULL, MAS_CLI_OPT_NOLISTEN},
  {"listen", no_argument, NULL, MAS_CLI_OPT_LISTEN},

  {"nodaemon", no_argument, NULL, MAS_CLI_OPT_NODAEMON},
  {"daemon", no_argument, NULL, MAS_CLI_OPT_DAEMON},

  {"proto", required_argument, NULL, MAS_CLI_OPT_PROTO},
  {"host", required_argument, NULL, MAS_CLI_OPT_HOST},
  {"nohosts", no_argument, NULL, MAS_CLI_OPT_NOHOSTS},
  {"port", required_argument, NULL, MAS_CLI_OPT_PORT},

  {"nomsg", no_argument, NULL, MAS_CLI_OPT_NOMSG},
  {"msg", required_argument, NULL, MAS_CLI_OPT_MSG},

  {NULL, 0, NULL, 0},
};

long
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

int
mas_cli_make_option( MAS_PASS_OPTS_DECLARE int opt, const char *m_optarg )
{
  MAS_PASS_OPTS_DECL_PREF;
  int r = 0;
  int v = 0;

  /* HMSG( "CLI M/O %d", opt ); */
  switch ( opt )
  {
  case MAS_CLI_OPT_TEST:
    HMSG( "Test" );
    break;
  case MAS_CLI_OPT_HELP:
    for ( int io = 0; io < sizeof( cli_longopts ) / sizeof( cli_longopts[0] ); io++ )
    {
      HMSG( "%s", cli_longopts[io].name );
    }
    break;
  case MAS_CLI_OPT_COMMAND:
    HMSG( "COMMAND %s", optarg );
    mas_ctrl_add_command( optarg );
    break;
  case MAS_CLI_OPT_MSGTO:
    HMSG( "MSG>%s", optarg );
    if ( MAS_PASS_OPTS_PREF msgfilename )
      mas_free( MAS_PASS_OPTS_PREF msgfilename );
    MAS_PASS_OPTS_PREF msgfilename = NULL;

    if ( optarg && *optarg )
      MAS_PASS_OPTS_PREF msgfilename = mas_strdup( optarg );

    break;
  case MAS_CLI_OPT_HOST:
    MAS_PASS_OPTS_PREF hostsv.c = mas_add_argv_arg( MAS_PASS_OPTS_PREF hostsv.c, &MAS_PASS_OPTS_PREF hostsv.v, optarg );
    mMSG( "HOST %d: %s [%p]", MAS_PASS_OPTS_PREF hostsv.c, optarg, ( void * ) MAS_PASS_OPTS_PREF hostsv.v );
    break;
  case MAS_CLI_OPT_PROTO:
    MAS_PASS_OPTS_PREF protosv.c = mas_add_argv_arg( MAS_PASS_OPTS_PREF protosv.c, &MAS_PASS_OPTS_PREF protosv.v, optarg );
    mMSG( "PROTO %d: %s [%p]", MAS_PASS_OPTS_PREF protosv.c, optarg, ( void * ) MAS_PASS_OPTS_PREF protosv.v );
    break;
  case MAS_CLI_OPT_PORT:
    if ( optarg && *optarg )
    {
      MAS_PASS_OPTS_PREF default_port = 0;

      sscanf( optarg, "%u", &MAS_PASS_OPTS_PREF default_port );
      /* HMSG( "PORT: %u", MAS_PASS_OPTS_PREF default_port ); */
    }
    break;
  case MAS_CLI_OPT_MODSDIR:
    if ( MAS_PASS_OPTS_PREF dir.mods )
      mas_free( MAS_PASS_OPTS_PREF dir.mods );
    MAS_PASS_OPTS_PREF dir.mods = NULL;

    if ( optarg && *optarg )
      MAS_PASS_OPTS_PREF dir.mods = mas_strdup( optarg );

    break;
  case MAS_CLI_OPT_PROTODIR:
    if ( MAS_PASS_OPTS_PREF dir.proto )
      mas_free( MAS_PASS_OPTS_PREF dir.proto );
    MAS_PASS_OPTS_PREF dir.proto = NULL;

    if ( optarg && *optarg )
      MAS_PASS_OPTS_PREF dir.proto = mas_strdup( optarg );

    break;
  case MAS_CLI_OPT_LOGDIR:
    if ( MAS_PASS_OPTS_PREF dir.log )
      mas_free( MAS_PASS_OPTS_PREF dir.log );
    MAS_PASS_OPTS_PREF dir.log = NULL;

    if ( optarg && *optarg )
      MAS_PASS_OPTS_PREF dir.log = mas_strdup( optarg );

    break;
  case MAS_CLI_OPT_HISTORYDIR:
    if ( MAS_PASS_OPTS_PREF dir.history )
      mas_free( MAS_PASS_OPTS_PREF dir.history );
    MAS_PASS_OPTS_PREF dir.history = NULL;

    if ( optarg && *optarg )
      MAS_PASS_OPTS_PREF dir.history = mas_strdup( optarg );

    break;
  case MAS_CLI_OPT_LISTENER_SINGLE:
    MAS_PASS_OPTS_PREF listener_single = 1;
    break;
  case MAS_CLI_OPT_TRANSACTION_SINGLE:
    MAS_PASS_OPTS_PREF transaction_single = 1;
    break;
  case MAS_CLI_OPT_NODAEMON:
    MAS_PASS_OPTS_PREF nodaemon = 1;
    break;
  case MAS_CLI_OPT_DAEMON:
    MAS_PASS_OPTS_PREF nodaemon = 0;
    break;
  case MAS_CLI_OPT_NOSINGLE_CHILD:
    MAS_PASS_OPTS_PREF single_child = 0;
    break;
  case MAS_CLI_OPT_SINGLE_CHILD:
    MAS_PASS_OPTS_PREF single_child = 1;
    break;
  case MAS_CLI_OPT_NOSINGLE_INSTANCE:
    MAS_PASS_OPTS_PREF single_instance = 0;
    break;
  case MAS_CLI_OPT_SINGLE_INSTANCE:
    MAS_PASS_OPTS_PREF single_instance = 1;
    break;
  case MAS_CLI_OPT_NOLOGGER:
    MAS_PASS_OPTS_PREF nologger = 1;
    break;
  case MAS_CLI_OPT_LOGGER:
    MAS_PASS_OPTS_PREF nologger = 0;
    break;
  case MAS_CLI_OPT_NOLOG:
    MAS_PASS_OPTS_PREF nolog = 1;
    break;
  case MAS_CLI_OPT_LOG:
    MAS_PASS_OPTS_PREF nolog = 0;
    break;
  case MAS_CLI_OPT_NOTICKER:
    MAS_PASS_OPTS_PREF noticker = 1;
    break;
  case MAS_CLI_OPT_TICKER:
    MAS_PASS_OPTS_PREF noticker = 0;
    break;
  case MAS_CLI_OPT_TICKER_MODE:
    if ( optarg && *optarg )
    {
      MAS_PASS_OPTS_PREF ticker_mode = 0;

      sscanf( optarg, "%u", &MAS_PASS_OPTS_PREF ticker_mode );
    }
    break;
  case MAS_CLI_OPT_NOREAD_HOME_OPTS:
    MAS_PASS_OPTS_PREF read_user_opts = 0;
    break;
  case MAS_CLI_OPT_READ_HOME_OPTS:
    MAS_PASS_OPTS_PREF read_user_opts = 1;
    break;
  case MAS_CLI_OPT_NOWATCHER:
    MAS_PASS_OPTS_PREF nowatcher = 1;
    break;
  case MAS_CLI_OPT_WATCHER:
    MAS_PASS_OPTS_PREF nowatcher = 0;
    break;
  case MAS_CLI_OPT_NOLISTENER:
    MAS_PASS_OPTS_PREF nolistener = mas_cli_optval( optarg, 30, &v );
    break;
  case MAS_CLI_OPT_LISTENER:
    MAS_PASS_OPTS_PREF nolistener = 0;
    break;
  case MAS_CLI_OPT_NOLISTEN:
    MAS_PASS_OPTS_PREF nolisten = mas_cli_optval( optarg, 30, &v );
    break;
  case MAS_CLI_OPT_LISTEN:
    MAS_PASS_OPTS_PREF nolisten = 0;
    break;
  case MAS_CLI_OPT_NOMASTER:
    MAS_PASS_OPTS_PREF nomaster = mas_cli_optval( optarg, 30, &v );
    break;
  case MAS_CLI_OPT_MASTER_THREAD:
    MAS_PASS_OPTS_PREF make_master_thread = 1;
    break;
  case MAS_CLI_OPT_NOMASTER_THREAD:
    MAS_PASS_OPTS_PREF make_master_thread = 0;
    break;
  case MAS_CLI_OPT_MASTER:
    MAS_PASS_OPTS_PREF nomaster = 0;
    break;
  case MAS_CLI_OPT_NOREDIRECT_STD:
    MAS_PASS_OPTS_PREF noredirect_std = 1;
    break;
  case MAS_CLI_OPT_REDIRECT_STD:
    MAS_PASS_OPTS_PREF noredirect_std = 0;
    break;
  case MAS_CLI_OPT_NOCLOSE_STD:
    MAS_PASS_OPTS_PREF noclose_std = 1;
    break;
  case MAS_CLI_OPT_CLOSE_STD:
    MAS_PASS_OPTS_PREF noclose_std = 0;
    break;
  case MAS_CLI_OPT_NOMESSAGES:
    MAS_PASS_OPTS_PREF nomessages = 1;
    break;
  case MAS_CLI_OPT_MESSAGES:
    MAS_PASS_OPTS_PREF nomessages = 0;
    break;
  case MAS_CLI_OPT_NOMSG:
    /* HMSG( "flags: %lo", MAS_PASS_OPTS_PREF f.word ); */
    MAS_PASS_OPTS_PREF f.word = 0;
    /* HMSG( "flags: %lo", MAS_PASS_OPTS_PREF f.word ); */
    MAS_PASS_OPTS_PREF f.bit.msg_trace = 0;

    /* HMSG( "flags: %lo", MAS_PASS_OPTS_PREF f.word ); */
    break;
  case MAS_CLI_OPT_MSG:
    if ( 0 == strcmp( "mem", optarg ) )
      MAS_PASS_OPTS_PREF f.bit.msg_mem = 1;

    break;
  case MAS_CLI_OPT_NOHOSTS:
    MAS_PASS_OPTS_PREF hostsv.c = mas_del_argv( MAS_PASS_OPTS_PREF hostsv.c, MAS_PASS_OPTS_PREF hostsv.v, 0 );
    MAS_PASS_OPTS_PREF hostsv.v = NULL;

    break;
  case MAS_CLI_OPT_EXITSLEEP:
    MAS_PASS_OPTS_PREF exitsleep = ( unsigned ) ( optarg && *optarg ? strtol( optarg, NULL, 10 ) : 30 );
    break;

  default:                     /* '?' ; ':' */
    /* fprintf( ctrl.stderrfile, "Usage: %s [-t nsecs] [-n] name\n", argv[0] ); */

    /* ctrl.in_client = 0; */
    ctrl.fatal = 1;
    ctrl.keep_listening = 0;
    HMSG( "CLI unknown opt:%d [%c]", opt, opt > ' ' && opt <= 'z' ? opt : '-' );
    IEVAL( r, -1 );
    break;
  }
  if ( v < 0 )
  {
    HMSG( "CLI wrong value '%s'", optarg );
    IEVAL( r, -1 );
  }
  /* HMSG( "getopt_long:%d Usage: %s [--help -h]", argv[0] ); */
  return r;
}

int
mas_cli_options( MAS_PASS_OPTS_DECLARE int argc, char *const argv[] )
{
  int r = 0;
  int opt;
  int indx = 0;
  int afterlast = 0;

  optind = 1;
  while ( r >= 0 && !ctrl.fatal && ( opt = getopt_long( argc, argv, cli_enabled_options, cli_longopts, &indx ) ) >= 0 )
  {
    /* HMSG( "CLI opt:%d: optind:%d err:%d / %d", opt, optind, opt == '?', opt == ':' ); */
    IEVAL( r, mas_cli_make_option( MAS_PASS_OPTS_PASS opt, optarg ) );
    afterlast = optind;
    /* HMSG( "(%d) CLI %d: %d", r, opt, optind ); */
  }
  MAS_LOG( "(%d) cli options made", r );
  return r < 0 ? r : afterlast;
}
