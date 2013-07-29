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



#include "mas_cli_opts.h"


/*
this:
  mas_cli_opts.c
related:
  mas_cli_opts.h
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
  MAS_CLI_OPT_INFO = 'i',
  MAS_CLI_OPT_QUIT = 'q',
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
  MAS_CLI_OPT_SYSDAEMON,
  MAS_CLI_OPT_NOSYSDAEMON,
  MAS_CLI_OPT_MSG,
  MAS_CLI_OPT_NOMSG,
  MAS_CLI_OPT_READ_HOME_OPTS,
  MAS_CLI_OPT_NOREAD_HOME_OPTS,
  MAS_CLI_OPT_NOHOSTS,
  MAS_CLI_OPT_NOPROTOS,
  MAS_CLI_OPT_REDIRECT_STD,
  MAS_CLI_OPT_NOREDIRECT_STD,
  MAS_CLI_OPT_CLOSE_STD,
  MAS_CLI_OPT_NOCLOSE_STD,
  MAS_CLI_OPT_MESSAGES_PARENT,
  MAS_CLI_OPT_MESSAGES_CHILD,
  MAS_CLI_OPT_MESSAGES,
  MAS_CLI_OPT_NOMESSAGES_PARENT,
  MAS_CLI_OPT_NOMESSAGES_CHILD,
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
  MAS_CLI_OPT_STDERRTO,
  MAS_CLI_OPT_STDOUTTO,
  MAS_CLI_OPT_SINGLE_INSTANCE,
  MAS_CLI_OPT_NOSINGLE_INSTANCE,
  MAS_CLI_OPT_SINGLE_CHILD,
  MAS_CLI_OPT_NOSINGLE_CHILD,
  MAS_CLI_OPT_SAVE_USER_OPTS,
  MAS_CLI_OPT_NOSAVE_USER_OPTS,
} mas_cli_opts_t;

/* thM:L:dH:P: */
 /* static char cli_enabled_options[] = "thM:L:dH:P:"; */

static struct option cli_longopts[] = {
  {"test", no_argument, NULL, MAS_CLI_OPT_TEST},
  {"help", no_argument, NULL, MAS_CLI_OPT_HELP},
  {"info", no_argument, NULL, MAS_CLI_OPT_INFO},
  {"quit", no_argument, NULL, MAS_CLI_OPT_QUIT},
  {"exitsleep", optional_argument, NULL, MAS_CLI_OPT_EXITSLEEP},

  {"single", no_argument, NULL, MAS_CLI_OPT_SINGLE_INSTANCE},
  {"single-instance", no_argument, NULL, MAS_CLI_OPT_SINGLE_INSTANCE},
  {"multi-instance", no_argument, NULL, MAS_CLI_OPT_NOSINGLE_INSTANCE},

  {"single-child", no_argument, NULL, MAS_CLI_OPT_SINGLE_CHILD},
  {"multi-child", no_argument, NULL, MAS_CLI_OPT_NOSINGLE_CHILD},

  {"save-user-opts", optional_argument, NULL, MAS_CLI_OPT_SAVE_USER_OPTS},
  {"nosave-user-opts", no_argument, NULL, MAS_CLI_OPT_NOSAVE_USER_OPTS},

  {"command", required_argument, NULL, MAS_CLI_OPT_COMMAND},
  {"redirect-messages", required_argument, NULL, MAS_CLI_OPT_MSGTO},
  {"redirect-stderr", required_argument, NULL, MAS_CLI_OPT_STDERRTO},
  {"redirect-stdout", required_argument, NULL, MAS_CLI_OPT_STDOUTTO},
  {"listener-single", no_argument, NULL, MAS_CLI_OPT_LISTENER_SINGLE},
  {"transaction-single", no_argument, NULL, MAS_CLI_OPT_TRANSACTION_SINGLE},

  {"parent-messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES_PARENT},
  {"noparent-messages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES_PARENT},

  {"child-messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES_CHILD},
  {"nochild-messages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES_CHILD},

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

  {"nosysdaemon", no_argument, NULL, MAS_CLI_OPT_NOSYSDAEMON},
  {"sysdaemon", no_argument, NULL, MAS_CLI_OPT_SYSDAEMON},

  {"nosys-daemon", no_argument, NULL, MAS_CLI_OPT_NOSYSDAEMON},
  {"sys-daemon", no_argument, NULL, MAS_CLI_OPT_SYSDAEMON},

  {"proto", required_argument, NULL, MAS_CLI_OPT_PROTO},
  {"noprotos", no_argument, NULL, MAS_CLI_OPT_NOPROTOS},
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
mas_cli_make_option( mas_options_t * popts, int opt, const char *m_optarg )
{
  EVAL_PREPARE;
  int r = 0;
  int v = 0;

  /* HMSG( "CLI M/O %d", opt ); */
  switch ( opt )
  {
  case MAS_CLI_OPT_TEST:
    HMSG( "Test" );
    popts->test = 1;
    break;
  case MAS_CLI_OPT_HELP:
    for ( int io = 0; io < sizeof( cli_longopts ) / sizeof( cli_longopts[0] ); io++ )
    {
      HMSG( "%s", cli_longopts[io].name );
    }
    break;
  case MAS_CLI_OPT_INFO:
    HMSG( "INFO" );
    popts->info = 1;
    break;
  case MAS_CLI_OPT_QUIT:
    HMSG( "QUIT" );
    popts->quit = 1;
    break;
  case MAS_CLI_OPT_COMMAND:
    HMSG( "COMMAND %s", m_optarg );
    mas_ctrl_add_command( m_optarg );
    break;
  case MAS_CLI_OPT_MSGTO:
    HMSG( "MSG>%s", m_optarg );
    if ( popts->msgfilename )
      mas_free( popts->msgfilename );
    popts->msgfilename = NULL;

    if ( m_optarg && *m_optarg )
      popts->msgfilename = mas_strdup( m_optarg );
    break;
  case MAS_CLI_OPT_STDERRTO:
    HMSG( "STDERR>%s", m_optarg );
    if ( popts->stderr_filename )
      mas_free( popts->stderr_filename );
    popts->stderr_filename = NULL;

    if ( m_optarg && *m_optarg )
      popts->stderr_filename = mas_strdup( m_optarg );
    break;
  case MAS_CLI_OPT_STDOUTTO:
    HMSG( "MSG>%s", m_optarg );
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
      /* HMSG( "PORT: %u", popts-> default_port ); */
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
    popts->listener_single = 1;
    break;
  case MAS_CLI_OPT_TRANSACTION_SINGLE:
    popts->transaction_single = 1;
    break;
  case MAS_CLI_OPT_NOSYSDAEMON:
    popts->daemon.sys = 0;
    break;
  case MAS_CLI_OPT_SYSDAEMON:
    popts->daemon.sys = 1;
    break;
  case MAS_CLI_OPT_NODAEMON:
    popts->daemon.disable = 1;
    break;
  case MAS_CLI_OPT_DAEMON:
    popts->daemon.disable = 0;
    break;
  case MAS_CLI_OPT_NOSAVE_USER_OPTS:
    popts->save_user_opts = 0;
    break;
  case MAS_CLI_OPT_SAVE_USER_OPTS:
    if ( popts->save_user_opts_filename )
      mas_free( popts->save_user_opts_filename );
    popts->save_user_opts_filename = NULL;
    if ( m_optarg && *m_optarg )
      popts->save_user_opts_filename = mas_strdup( m_optarg );
    popts->save_user_opts = 1;
    break;
  case MAS_CLI_OPT_NOSINGLE_CHILD:
    popts->single_child = 0;
    break;
  case MAS_CLI_OPT_SINGLE_CHILD:
    popts->single_child = 1;
    break;
  case MAS_CLI_OPT_NOSINGLE_INSTANCE:
    popts->single_instance = 0;
    break;
  case MAS_CLI_OPT_SINGLE_INSTANCE:
    popts->single_instance = 1;
    break;
  case MAS_CLI_OPT_NOLOGGER:
    popts->log.run = 0;
    break;
  case MAS_CLI_OPT_LOGGER:
    popts->log.run = 1;
    break;
  case MAS_CLI_OPT_NOLOG:
    popts->log.enable = 0;
    break;
  case MAS_CLI_OPT_LOG:
    popts->log.enable = 1;
    break;
  case MAS_CLI_OPT_NOTICKER:
    popts->noticker = 1;
    break;
  case MAS_CLI_OPT_TICKER:
    popts->noticker = 0;
    break;
  case MAS_CLI_OPT_TICKER_MODE:
    if ( m_optarg && *m_optarg )
    {
      popts->ticker_mode = 0;

      sscanf( m_optarg, "%u", &popts->ticker_mode );
    }
    break;
  case MAS_CLI_OPT_NOREAD_HOME_OPTS:
    popts->read_user_opts = 0;
    break;
  case MAS_CLI_OPT_READ_HOME_OPTS:
    popts->read_user_opts = 1;
    break;
  case MAS_CLI_OPT_NOWATCHER:
    popts->nowatcher = 1;
    break;
  case MAS_CLI_OPT_WATCHER:
    popts->nowatcher = 0;
    break;
  case MAS_CLI_OPT_NOLISTENER:
    popts->nolistener = mas_cli_optval( m_optarg, 30, &v );
    break;
  case MAS_CLI_OPT_LISTENER:
    popts->nolistener = 0;
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
    popts->make_master_thread = 1;
    break;
  case MAS_CLI_OPT_NOMASTER_THREAD:
    popts->make_master_thread = 0;
    break;
  case MAS_CLI_OPT_MASTER:
    popts->nomaster = 0;
    break;
  case MAS_CLI_OPT_NOREDIRECT_STD:
    popts->daemon.disable_redirect_std = 1;
    break;
  case MAS_CLI_OPT_REDIRECT_STD:
    popts->daemon.disable_redirect_std = 0;
    break;
  case MAS_CLI_OPT_NOCLOSE_STD:
    popts->daemon.disable_close_std = 1;
    break;
  case MAS_CLI_OPT_CLOSE_STD:
    popts->daemon.disable_close_std = 0;
    break;
  case MAS_CLI_OPT_NOMESSAGES_PARENT:
    popts->nomessages_parent = 1;
    break;
  case MAS_CLI_OPT_MESSAGES_PARENT:
    popts->nomessages_parent = 0;
    break;
  case MAS_CLI_OPT_NOMESSAGES_CHILD:
    popts->nomessages_child = 1;
    break;
  case MAS_CLI_OPT_MESSAGES_CHILD:
    popts->nomessages_child = 0;
    break;
  case MAS_CLI_OPT_NOMESSAGES:
    popts->nomessages = 1;
    break;
  case MAS_CLI_OPT_MESSAGES:
    popts->nomessages = 0;
    break;
  case MAS_CLI_OPT_NOMSG:
    /* HMSG( "flags: %lo", popts-> f.word ); */
    popts->f.word = 0;
    /* HMSG( "flags: %lo", popts-> f.word ); */
    popts->f.bit.msg_trace = 0;

    /* HMSG( "flags: %lo", popts-> f.word ); */
    break;
  case MAS_CLI_OPT_MSG:
    if ( 0 == strcmp( "mem", m_optarg ) )
      popts->f.bit.msg_mem = 1;

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
      /* fprintf( ctrl.stderrfile, "Usage: %s [-t nsecs] [-n] name\n", argv[0] ); */

      /* ctrl.in_client = 0; */
      ctrl.fatal = 1;
      ctrl.keep_listening = 0;
      HMSG( "CLI unknown opt:%d [%c]", opt, opt > ' ' && opt <= 'z' ? opt : '-' );
      IEVAL( r, -1 );
    }
    break;
  }
  if ( v < 0 )
  {
    HMSG( "CLI wrong value '%s'", m_optarg );
    IEVAL( r, -1 );
  }
  /* HMSG( "getopt_long:%d Usage: %s [--help -h]", argv[0] ); */
  return r;
}

char *
mas_cli_enabled_options( void )
{
  /* struct option       */
  /* {                   */
  /*   const char *name; */
  /*   int has_arg;      */
  /*   int *flag;        */
  /*   int val;          */
  /* };                  */
  char *s = NULL;

  for ( int io = 0; io < sizeof( cli_longopts ) / sizeof( cli_longopts[0] ); io++ )
  {
    char opt[10];

    if ( cli_longopts[io].val < MAS_CLI_OPT_PLUS )
    {
      opt[0] = ( char ) cli_longopts[io].val;
      opt[1] = 0;
      opt[2] = 0;
      if ( cli_longopts[io].has_arg == required_argument /* || cli_longopts[io].has_arg == optional_argument */  )
        opt[1] = ':';
      s = mas_strcat_x( s, opt );
    }
  }
  return s;
}

int
mas_cli_options_init( mas_options_t * popts, const char **message )
{
  int r = -1;
  int argc;
  char *const *argv;

  if ( popts )
  {
    CTRL_PREPARE;
    argc = popts->argvv.c;
    argv = popts->argvv.v;
    r = _mas_cli_options( popts, argc, argv );
    ctrl.argv_nonoptind = r;
  }
  if ( message )
    *message = __func__;

  HMSG( "(%d) INIT CLI", r );
  return r >= 0 ? 0 : r;
}

int
_mas_cli_options( mas_options_t * popts, int argc, char *const *argv )
{
  CTRL_PREPARE;
  int r = 0;
  int opt;
  int indx = 0;
  int afterlast = 0;
  char *enabled_opts;

  optind = 1;
  enabled_opts = mas_cli_enabled_options(  );
  while ( r >= 0 && !ctrl.fatal && ( opt = getopt_long( argc, argv, enabled_opts, cli_longopts, &indx ) ) >= 0 )
  {
    /* HMSG( "CLI opt:%d: optind:%d err:%d / %d", opt, optind, opt == '?', opt == ':' ); */
    IEVAL( r, mas_cli_make_option( popts, opt, optarg ) );
    afterlast = optind;
    /* HMSG( "(%d) CLI %d: %d", r, opt, optind ); */
  }
  MAS_LOG( "(%d) cli options made", r );
  mas_free( enabled_opts );
  return r < 0 ? r : afterlast;
}
