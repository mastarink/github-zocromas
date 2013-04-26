#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/control/mas_control.h>

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
mas_cli_make_option( int opt, const char *m_optarg )
{
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
    if ( opts.msgfilename )
      mas_free( opts.msgfilename );
    opts.msgfilename = NULL;
    if ( optarg && *optarg )
      opts.msgfilename = mas_strdup( optarg );
    break;
  case MAS_CLI_OPT_HOST:
    opts.hostsv.c = mas_add_argv_arg( opts.hostsv.c, &opts.hostsv.v, optarg );
    mMSG( "HOST %d: %s [%p]", opts.hostsv.c, optarg, ( void * ) opts.hostsv.v );
    break;
  case MAS_CLI_OPT_PROTO:
    opts.protosv.c = mas_add_argv_arg( opts.protosv.c, &opts.protosv.v, optarg );
    mMSG( "PROTO %d: %s [%p]", opts.protosv.c, optarg, ( void * ) opts.protosv.v );
    break;
  case MAS_CLI_OPT_PORT:
    if ( optarg && *optarg )
    {
      opts.default_port = 0;
      sscanf( optarg, "%u", &opts.default_port );
      /* HMSG( "PORT: %u", opts.default_port ); */
    }
    break;
  case MAS_CLI_OPT_MODSDIR:
    if ( opts.dir.mods )
      mas_free( opts.dir.mods );
    opts.dir.mods = NULL;
    if ( optarg && *optarg )
      opts.dir.mods = mas_strdup( optarg );
    break;
  case MAS_CLI_OPT_PROTODIR:
    if ( opts.dir.proto )
      mas_free( opts.dir.proto );
    opts.dir.proto = NULL;
    if ( optarg && *optarg )
      opts.dir.proto = mas_strdup( optarg );
    break;
  case MAS_CLI_OPT_LOGDIR:
    if ( opts.dir.log )
      mas_free( opts.dir.log );
    opts.dir.log = NULL;
    if ( optarg && *optarg )
      opts.dir.log = mas_strdup( optarg );
    break;
  case MAS_CLI_OPT_HISTORYDIR:
    if ( opts.dir.history )
      mas_free( opts.dir.history );
    opts.dir.history = NULL;
    if ( optarg && *optarg )
      opts.dir.history = mas_strdup( optarg );
    break;
  case MAS_CLI_OPT_LISTENER_SINGLE:
    opts.listener_single = 1;
    break;
  case MAS_CLI_OPT_TRANSACTION_SINGLE:
    opts.transaction_single = 1;
    break;
  case MAS_CLI_OPT_NODAEMON:
    opts.nodaemon = 1;
    break;
  case MAS_CLI_OPT_DAEMON:
    opts.nodaemon = 0;
    break;
  case MAS_CLI_OPT_NOSINGLE_CHILD:
    opts.single_child = 0;
    break;
  case MAS_CLI_OPT_SINGLE_CHILD:
    opts.single_child = 1;
    break;
  case MAS_CLI_OPT_NOSINGLE_INSTANCE:
    opts.single_instance = 0;
    break;
  case MAS_CLI_OPT_SINGLE_INSTANCE:
    opts.single_instance = 1;
    break;
  case MAS_CLI_OPT_NOLOGGER:
    opts.nologger = 1;
    break;
  case MAS_CLI_OPT_LOGGER:
    opts.nologger = 0;
    break;
  case MAS_CLI_OPT_NOLOG:
    opts.nolog = 1;
    break;
  case MAS_CLI_OPT_LOG:
    opts.nolog = 0;
    break;
  case MAS_CLI_OPT_NOTICKER:
    opts.noticker = 1;
    break;
  case MAS_CLI_OPT_TICKER:
    opts.noticker = 0;
    break;
  case MAS_CLI_OPT_TICKER_MODE:
    if ( optarg && *optarg )
    {
      opts.ticker_mode = 0;
      sscanf( optarg, "%u", &opts.ticker_mode );
    }
    break;
  case MAS_CLI_OPT_NOWATCHER:
    opts.nowatcher = 1;
    break;
  case MAS_CLI_OPT_WATCHER:
    opts.nowatcher = 0;
    break;
  case MAS_CLI_OPT_NOLISTENER:
    opts.nolistener = mas_cli_optval( optarg, 30, &v );
    break;
  case MAS_CLI_OPT_LISTENER:
    opts.nolistener = 0;
    break;
  case MAS_CLI_OPT_NOLISTEN:
    opts.nolisten = mas_cli_optval( optarg, 30, &v );
    break;
  case MAS_CLI_OPT_LISTEN:
    opts.nolisten = 0;
    break;
  case MAS_CLI_OPT_NOMASTER:
    opts.nomaster = mas_cli_optval( optarg, 30, &v );
    break;
  case MAS_CLI_OPT_MASTER_THREAD:
    opts.make_master_thread = 1;
    break;
  case MAS_CLI_OPT_NOMASTER_THREAD:
    opts.make_master_thread = 0;
    break;
  case MAS_CLI_OPT_MASTER:
    opts.nomaster = 0;
    break;
  case MAS_CLI_OPT_NOREDIRECT_STD:
    opts.noredirect_std = 1;
    break;
  case MAS_CLI_OPT_REDIRECT_STD:
    opts.noredirect_std = 0;
    break;
  case MAS_CLI_OPT_NOCLOSE_STD:
    opts.noclose_std = 1;
    break;
  case MAS_CLI_OPT_CLOSE_STD:
    opts.noclose_std = 0;
    break;
  case MAS_CLI_OPT_NOMESSAGES:
    opts.nomessages = 1;
    break;
  case MAS_CLI_OPT_MESSAGES:
    opts.nomessages = 0;
    break;
  case MAS_CLI_OPT_NOMSG:
    /* HMSG( "flags: %lo", opts.f.word ); */
    opts.f.word = 0;
    /* HMSG( "flags: %lo", opts.f.word ); */
    opts.f.bit.msg_trace = 0;
    /* HMSG( "flags: %lo", opts.f.word ); */
    break;
  case MAS_CLI_OPT_MSG:
    if ( 0 == strcmp( "mem", optarg ) )
      opts.f.bit.msg_mem = 1;
    break;
  case MAS_CLI_OPT_NOHOSTS:
    opts.hostsv.c = mas_del_argv( opts.hostsv.c, opts.hostsv.v, 0 );
    opts.hostsv.v = NULL;
    break;
  case MAS_CLI_OPT_EXITSLEEP:
    opts.exitsleep = ( unsigned ) ( optarg && *optarg ? strtol( optarg, NULL, 10 ) : 30 );
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
mas_cli_options( int argc, char *const argv[] )
{
  int r = 0;
  int opt;
  int indx = 0;
  int afterlast = 0;

  optind = 1;
  while ( r >= 0 && !ctrl.fatal && ( opt = getopt_long( argc, argv, cli_enabled_options, cli_longopts, &indx ) ) >= 0 )
  {
    /* HMSG( "CLI opt:%d: optind:%d err:%d / %d", opt, optind, opt == '?', opt == ':' ); */
    IEVAL( r, mas_cli_make_option( opt, optarg ) );
    afterlast = optind;
    /* HMSG( "(%d) CLI %d: %d", r, opt, optind ); */
  }
  MAS_LOG( "(%d) cli options made", r );
  return r < 0 ? r : afterlast;
}
