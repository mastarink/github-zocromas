#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <getopt.h>

/* #include <mastar/wrap/mas_memory.h> */
/* #include <mastar/tools/mas_arg_tools.h> */
#include <mastar/tools/mas_tools.h>

/* #include <mastar/msg/mas_msg_def.h> */
/* #include <mastar/msg/mas_msg_tools.h> */
/* #include <mastar/log/mas_log.h> */

/* #include <mastar/control/mas_control.h> */
/* #include <mastar/types/mas_control_types.h> */
#include <mastar/types/mas_opts_types.h>



#include "mas_cli_opts_def.h"
#include "mas_cli_opts_data.h"


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



/* thM:L:dH:P: */
 /* static char cli_enabled_options[] = "thM:L:dH:P:"; */

struct option mas_cli_longopts_table[] = {
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

  {"nopidfile", no_argument, NULL, MAS_CLI_OPT_NOPIDFILE},
  {"pidfile", no_argument, NULL, MAS_CLI_OPT_PIDFILE},

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
  {"init-msg", optional_argument, NULL, MAS_CLI_OPT_INIT_MSG},
  {"init-message", required_argument, NULL, MAS_CLI_OPT_INIT_MSG},

  {NULL, 0, NULL, 0},
};

int
mas_cli_options_data_init( mas_options_t * popts, const char **message )
{
  if ( popts )
  {
    popts->cli_longopts = mas_cli_longopts_table;
    popts->cli_longopts_num = sizeof( mas_cli_longopts_table ) / sizeof( mas_cli_longopts_table[0] );
  }
  return 0;
}

__attribute__ ( ( constructor( 3011 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s\n", __FILE__ ); */
  mas_common_constructor( IL, 1 );
}
