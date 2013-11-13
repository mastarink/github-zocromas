#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

/* #include <getopt.h> */

#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_tools.h>

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

/* struct option *mas_cli_longopts_table = NULL; */

struct option mas_cli_longopts_ctable[] = {
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

  {"noread-user-config", no_argument, NULL, MAS_CLI_OPT_NOREAD_USER_OPTS},
  {"read-user-config", no_argument, NULL, MAS_CLI_OPT_READ_USER_OPTS},

  {"noread-user-config-plus", no_argument, NULL, MAS_CLI_OPT_NOREAD_USER_OPTS_PLUS},
  {"read-user-config-plus", no_argument, NULL, MAS_CLI_OPT_READ_USER_OPTS_PLUS},

  {"nooverwrite-user-config", no_argument, NULL, MAS_CLI_OPT_NOOVERWRITE_USER_OPTS},
  {"overwrite-user-config", no_argument, NULL, MAS_CLI_OPT_OVERWRITE_USER_OPTS},

  {"nooverwrite-user-config-plus", no_argument, NULL, MAS_CLI_OPT_NOOVERWRITE_USER_OPTS_PLUS},
  {"overwrite-user-config-plus", no_argument, NULL, MAS_CLI_OPT_OVERWRITE_USER_OPTS_PLUS},

  {"nowatcher", no_argument, NULL, MAS_CLI_OPT_NOWATCHER},
  {"watcher", no_argument, NULL, MAS_CLI_OPT_NONOWATCHER},

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
  {"daemon", no_argument, NULL, MAS_CLI_OPT_NONODAEMON},

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

mas_optionx_t mas_cli_optx_table[] = {
  {{"test", no_argument, NULL, MAS_CLI_OPT_TEST}
   ,.optx_type = OPTX_TYPE_FLAG}
  , {{"help", no_argument, NULL, MAS_CLI_OPT_HELP}
     ,.optx_type = OPTX_TYPE_SPECIAL}
  , {{"info", no_argument, NULL, MAS_CLI_OPT_INFO}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"quit", no_argument, NULL, MAS_CLI_OPT_QUIT}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"exitsleep", optional_argument, NULL, MAS_CLI_OPT_EXITSLEEP}
     ,.optx_type = OPTX_TYPE_UNSIGNED}
  , {{"single", no_argument, NULL, MAS_CLI_OPT_SINGLE_INSTANCE}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"single-instance", no_argument, NULL, MAS_CLI_OPT_SINGLE_INSTANCE}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"multi-instance", no_argument, NULL, MAS_CLI_OPT_NOSINGLE_INSTANCE}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"single-child", no_argument, NULL, MAS_CLI_OPT_SINGLE_CHILD}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"multi-child", no_argument, NULL, MAS_CLI_OPT_NOSINGLE_CHILD}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"save-user-opts", optional_argument, NULL, MAS_CLI_OPT_SAVE_USER_OPTS}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nosave-user-opts", no_argument, NULL, MAS_CLI_OPT_NOSAVE_USER_OPTS}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"command", required_argument, NULL, MAS_CLI_OPT_COMMAND}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"redirect-messages", required_argument, NULL, MAS_CLI_OPT_MSGTO}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"redirect-stderr", required_argument, NULL, MAS_CLI_OPT_STDERRTO}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"redirect-stdout", required_argument, NULL, MAS_CLI_OPT_STDOUTTO}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"listener-single", no_argument, NULL, MAS_CLI_OPT_LISTENER_SINGLE}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"transaction-single", no_argument, NULL, MAS_CLI_OPT_TRANSACTION_SINGLE}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"parent-messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES_PARENT}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"noparent-messages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES_PARENT}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"child-messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES_CHILD}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nochild-messages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES_CHILD}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"nomessages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"redirect-std", no_argument, NULL, MAS_CLI_OPT_REDIRECT_STD}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"noredirect-std", no_argument, NULL, MAS_CLI_OPT_NOREDIRECT_STD}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"close-std", no_argument, NULL, MAS_CLI_OPT_CLOSE_STD}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"noclose-std", no_argument, NULL, MAS_CLI_OPT_NOCLOSE_STD}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"logger", no_argument, NULL, MAS_CLI_OPT_LOGGER}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"nologger", no_argument, NULL, MAS_CLI_OPT_NOLOGGER}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"log", no_argument, NULL, MAS_CLI_OPT_LOG}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nolog", no_argument, NULL, MAS_CLI_OPT_NOLOG}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"modsdir", required_argument, NULL, MAS_CLI_OPT_MODSDIR}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"protodir", required_argument, NULL, MAS_CLI_OPT_PROTODIR}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"logdir", required_argument, NULL, MAS_CLI_OPT_LOGDIR}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"historydir", required_argument, NULL, MAS_CLI_OPT_HISTORYDIR}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"read-user-config", no_argument, NULL, MAS_CLI_OPT_READ_USER_OPTS}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"noread-user-config", no_argument, NULL, MAS_CLI_OPT_NOREAD_USER_OPTS}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"read-user-config-plus", no_argument, NULL, MAS_CLI_OPT_READ_USER_OPTS_PLUS}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"noread-user-config-plus", no_argument, NULL, MAS_CLI_OPT_NOREAD_USER_OPTS_PLUS}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"overwrite-user-config", no_argument, NULL, MAS_CLI_OPT_OVERWRITE_USER_OPTS}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nooverwrite-user-config", no_argument, NULL, MAS_CLI_OPT_NOOVERWRITE_USER_OPTS}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"overwrite-user-config-plus", no_argument, NULL, MAS_CLI_OPT_OVERWRITE_USER_OPTS_PLUS}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nooverwrite-user-config-plus", no_argument, NULL, MAS_CLI_OPT_NOOVERWRITE_USER_OPTS_PLUS}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"watcher", no_argument, NULL, MAS_CLI_OPT_NONOWATCHER}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"nowatcher", no_argument, NULL, MAS_CLI_OPT_NOWATCHER}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"ticker", no_argument, NULL, MAS_CLI_OPT_TICKER}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"noticker", no_argument, NULL, MAS_CLI_OPT_NOTICKER}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"set-ticker-mode", required_argument, NULL, MAS_CLI_OPT_TICKER_MODE}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"pidfile", no_argument, NULL, MAS_CLI_OPT_PIDFILE}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nopidfile", no_argument, NULL, MAS_CLI_OPT_NOPIDFILE}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"master", no_argument, NULL, MAS_CLI_OPT_MASTER}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nomaster", required_argument, NULL, MAS_CLI_OPT_NOMASTER}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"mthread", no_argument, NULL, MAS_CLI_OPT_MASTER_THREAD}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nomthread", no_argument, NULL, MAS_CLI_OPT_NOMASTER_THREAD}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"listener", no_argument, NULL, MAS_CLI_OPT_LISTENER}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nolistener", required_argument, NULL, MAS_CLI_OPT_NOLISTENER}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"listen", no_argument, NULL, MAS_CLI_OPT_LISTEN}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nolisten", required_argument, NULL, MAS_CLI_OPT_NOLISTEN}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"daemon", no_argument, NULL, MAS_CLI_OPT_NONODAEMON}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nodaemon", no_argument, NULL, MAS_CLI_OPT_NODAEMON}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"sysdaemon", no_argument, NULL, MAS_CLI_OPT_SYSDAEMON}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nosysdaemon", no_argument, NULL, MAS_CLI_OPT_NOSYSDAEMON}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"sys-daemon", no_argument, NULL, MAS_CLI_OPT_SYSDAEMON}
     ,.optx_type = OPTX_TYPE_FLAG}
  , {{"nosys-daemon", no_argument, NULL, MAS_CLI_OPT_NOSYSDAEMON}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"proto", required_argument, NULL, MAS_CLI_OPT_PROTO}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"noprotos", no_argument, NULL, MAS_CLI_OPT_NOPROTOS}
     ,.optx_type = OPTX_TYPE_SPECIAL}
  , {{"host", required_argument, NULL, MAS_CLI_OPT_HOST}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"nohosts", no_argument, NULL, MAS_CLI_OPT_NOHOSTS}
     ,.optx_type = OPTX_TYPE_SPECIAL}
  , {{"port", required_argument, NULL, MAS_CLI_OPT_PORT}
     ,.optx_type = OPTX_TYPE_UNSIGNED}
  , {{"msg", required_argument, NULL, MAS_CLI_OPT_MSG}
     ,.optx_type = OPTX_TYPE_FLAG /* ?? */ }
  , {{"nomsg", no_argument, NULL, MAS_CLI_OPT_NOMSG}
     ,.optx_type = OPTX_TYPE_NOFLAG}
  , {{"init-msg", optional_argument, NULL, MAS_CLI_OPT_INIT_MSG}
     ,.optx_type = OPTX_TYPE_STR}
  , {{"init-message", required_argument, NULL, MAS_CLI_OPT_INIT_MSG}
     ,.optx_type = OPTX_TYPE_STR}
  , {{NULL, 0, NULL, 0}
     ,.optx_type = OPTX_TYPE_NONE}
};


mas_optionx_t *
mas_cli_find_optx( int opt )
{
  mas_optionx_t *optx = NULL;

  return optx;
}


struct option *
mas_cli_create_longopts_table( mas_optionx_t * table, size_t num )
{
  struct option *longtab = NULL;

  longtab = mas_malloc( sizeof( struct option ) * num );
  for ( int i = 0; i < num; i++ )
  {
    longtab[i] = table[i].longopt;
    longtab[i].name = mas_strdup( table[i].longopt.name );
  }
  return longtab;
}

int
mas_cli_options_data_init( mas_options_t * popts, const char **message )
{
  if ( popts )
  {
    popts->cli_longopts_num = sizeof( mas_cli_optx_table ) / sizeof( mas_cli_optx_table[0] );
    popts->cli_longopts = mas_cli_create_longopts_table( mas_cli_optx_table, popts->cli_longopts_num );
  }
  return 0;
}

void
mas_cli_delete_longopts_table( struct option *longtab, size_t num )
{
  for ( int i = 0; i < num; i++ )
  {
    mas_free( ( char * ) longtab[i].name );
  }
  mas_free( longtab );
}

void
mas_cli_options_data_destroy( mas_options_t * popts )
{
  if ( popts )
  {
    mas_cli_delete_longopts_table( popts->cli_longopts, popts->cli_longopts_num );
    popts->cli_longopts_num = 0;
    popts->cli_longopts = NULL;
  }
}

__attribute__ ( ( constructor( 3011 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s\n", __FILE__ ); */
  mas_common_constructor( IL, 1 );
}
