#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

/* #include <getopt.h> */

#include <mastar/wrap/mas_memory.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

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

#if 0
/* struct option mas_cli_longopts_ctable[] = {                                                    */
/*   {"test", no_argument, NULL, MAS_CLI_OPT_TEST},                                               */
/*   {"help", no_argument, NULL, MAS_CLI_OPT_HELP},                                               */
/*   {"info", no_argument, NULL, MAS_CLI_OPT_INFO},                                               */
/*   {"quit", no_argument, NULL, MAS_CLI_OPT_QUIT},                                               */
/*   {"exitsleep", optional_argument, NULL, MAS_CLI_OPT_EXITSLEEP},                               */
/*                                                                                                */
/*   {"single", no_argument, NULL, MAS_CLI_OPT_SINGLE_INSTANCE},                                  */
/*   {"single-instance", no_argument, NULL, MAS_CLI_OPT_SINGLE_INSTANCE},                         */
/*   {"multi-instance", no_argument, NULL, MAS_CLI_OPT_NOSINGLE_INSTANCE},                        */
/*                                                                                                */
/*   {"single-child", no_argument, NULL, MAS_CLI_OPT_SINGLE_CHILD},                               */
/*   {"multi-child", no_argument, NULL, MAS_CLI_OPT_NOSINGLE_CHILD},                              */
/*                                                                                                */
/*   {"save-user-opts", optional_argument, NULL, MAS_CLI_OPT_SAVE_USER_OPTS},                     */
/*   {"nosave-user-opts", no_argument, NULL, MAS_CLI_OPT_NOSAVE_USER_OPTS},                       */
/*                                                                                                */
/*   {"command", required_argument, NULL, MAS_CLI_OPT_COMMAND},                                   */
/*   {"redirect-messages", required_argument, NULL, MAS_CLI_OPT_MSGTO},                           */
/*   {"redirect-stderr", required_argument, NULL, MAS_CLI_OPT_STDERRTO},                          */
/*   {"redirect-stdout", required_argument, NULL, MAS_CLI_OPT_STDOUTTO},                          */
/*   {"listener-single", no_argument, NULL, MAS_CLI_OPT_LISTENER_SINGLE},                         */
/*   {"transaction-single", no_argument, NULL, MAS_CLI_OPT_TRANSACTION_SINGLE},                   */
/*                                                                                                */
/*   {"parent-messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES_PARENT},                         */
/*   {"noparent-messages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES_PARENT},                     */
/*                                                                                                */
/*   {"child-messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES_CHILD},                           */
/*   {"nochild-messages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES_CHILD},                       */
/*                                                                                                */
/*   {"messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES},                                       */
/*   {"nomessages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES},                                   */
/*                                                                                                */
/*   {"noredirect-std", no_argument, NULL, MAS_CLI_OPT_NOREDIRECT_STD},                           */
/*   {"redirect-std", no_argument, NULL, MAS_CLI_OPT_REDIRECT_STD},                               */
/*                                                                                                */
/*   {"noclose-std", no_argument, NULL, MAS_CLI_OPT_NOCLOSE_STD},                                 */
/*   {"close-std", no_argument, NULL, MAS_CLI_OPT_CLOSE_STD},                                     */
/*                                                                                                */
/*   {"nologger", no_argument, NULL, MAS_CLI_OPT_NOLOGGER},                                       */
/*   {"logger", no_argument, NULL, MAS_CLI_OPT_LOGGER},                                           */
/*                                                                                                */
/*   {"nolog", no_argument, NULL, MAS_CLI_OPT_NOLOG},                                             */
/*   {"log", no_argument, NULL, MAS_CLI_OPT_LOG},                                                 */
/*                                                                                                */
/*   {"modsdir", required_argument, NULL, MAS_CLI_OPT_MODSDIR},                                   */
/*   {"protodir", required_argument, NULL, MAS_CLI_OPT_PROTODIR},                                 */
/*   {"logdir", required_argument, NULL, MAS_CLI_OPT_LOGDIR},                                     */
/*   {"historydir", required_argument, NULL, MAS_CLI_OPT_HISTORYDIR},                             */
/*                                                                                                */
/*   {"noread-user-config", no_argument, NULL, MAS_CLI_OPT_NOREAD_USER_OPTS},                     */
/*   {"read-user-config", no_argument, NULL, MAS_CLI_OPT_READ_USER_OPTS},                         */
/*                                                                                                */
/*   {"noread-user-config-plus", no_argument, NULL, MAS_CLI_OPT_NOREAD_USER_OPTS_PLUS},           */
/*   {"read-user-config-plus", no_argument, NULL, MAS_CLI_OPT_READ_USER_OPTS_PLUS},               */
/*                                                                                                */
/*   {"nooverwrite-user-config", no_argument, NULL, MAS_CLI_OPT_NOOVERWRITE_USER_OPTS},           */
/*   {"overwrite-user-config", no_argument, NULL, MAS_CLI_OPT_OVERWRITE_USER_OPTS},               */
/*                                                                                                */
/*   {"nooverwrite-user-config-plus", no_argument, NULL, MAS_CLI_OPT_NOOVERWRITE_USER_OPTS_PLUS}, */
/*   {"overwrite-user-config-plus", no_argument, NULL, MAS_CLI_OPT_OVERWRITE_USER_OPTS_PLUS},     */
/*                                                                                                */
/*   {"nowatcher", no_argument, NULL, MAS_CLI_OPT_WOWATCHER},                                     */
/*   {"watcher", no_argument, NULL, MAS_CLI_OPT_NOWOWATCHER},                                     */
/*                                                                                                */
/*   {"noticker", no_argument, NULL, MAS_CLI_OPT_WOTICKER},                                       */
/*   {"ticker", no_argument, NULL, MAS_CLI_OPT_NOWOTICKER},                                       */
/*   {"ticker-mode", required_argument, NULL, MAS_CLI_OPT_TICKER_MODE},                       */
/*                                                                                                */
/*   {"nopidfile", no_argument, NULL, MAS_CLI_OPT_NOPIDFILE},                                     */
/*   {"pidfile", no_argument, NULL, MAS_CLI_OPT_PIDFILE},                                         */
/*                                                                                                */
/*   {"nomaster", required_argument, NULL, MAS_CLI_OPT_WOMASTER},                                 */
/*   {"master", no_argument, NULL, MAS_CLI_OPT_NOWOMASTER},                                       */
/*                                                                                                */
/*   {"nomthread", no_argument, NULL, MAS_CLI_OPT_NOMASTER_THREAD},                               */
/*   {"mthread", no_argument, NULL, MAS_CLI_OPT_MASTER_THREAD},                                   */
/*                                                                                                */
/*   {"nolistener", required_argument, NULL, MAS_CLI_OPT_WOLISTENER},                             */
/*   {"listener", no_argument, NULL, MAS_CLI_OPT_NOWOLISTENER},                                   */
/*                                                                                                */
/*   {"nolisten", required_argument, NULL, MAS_CLI_OPT_WOLISTEN},                                 */
/*   {"listen", no_argument, NULL, MAS_CLI_OPT_NOWOLISTEN},                                       */
/*                                                                                                */
/*   {"nodaemon", no_argument, NULL, MAS_CLI_OPT_WODAEMON},                                       */
/*   {"daemon", no_argument, NULL, MAS_CLI_OPT_NOWODAEMON},                                       */
/*                                                                                                */
/*   {"nosysdaemon", no_argument, NULL, MAS_CLI_OPT_NOSYSDAEMON},                                 */
/*   {"sysdaemon", no_argument, NULL, MAS_CLI_OPT_SYSDAEMON},                                     */
/*                                                                                                */
/*   {"nosys-daemon", no_argument, NULL, MAS_CLI_OPT_NOSYSDAEMON},                                */
/*   {"sys-daemon", no_argument, NULL, MAS_CLI_OPT_SYSDAEMON},                                    */
/*                                                                                                */
/*   {"proto", required_argument, NULL, MAS_CLI_OPT_PROTO},                                       */
/*   {"noprotos", no_argument, NULL, MAS_CLI_OPT_NOPROTOS},                                       */
/*   {"host", required_argument, NULL, MAS_CLI_OPT_HOST},                                         */
/*   {"nohosts", no_argument, NULL, MAS_CLI_OPT_NOHOSTS},                                         */
/*   {"port", required_argument, NULL, MAS_CLI_OPT_PORT},                                         */
/*                                                                                                */
/*   {"nomsg", no_argument, NULL, MAS_CLI_OPT_NOMSG},                                             */
/*   {"msg", required_argument, NULL, MAS_CLI_OPT_MSG},                                           */
/*   {"init-msg", optional_argument, NULL, MAS_CLI_OPT_INIT_MSG},                                 */
/*   {"init-message", required_argument, NULL, MAS_CLI_OPT_INIT_MSG},                             */
/*                                                                                                */
/*   {NULL, 0, NULL, 0},                                                                          */
/* };                                                                                             */
#endif

mas_optionx_t mas_cli_optx_table[] = {
  {{"test", no_argument, NULL, MAS_CLI_OPT_TEST}
   ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_TEST}
  , {{"help", no_argument, NULL, MAS_CLI_OPT_HELP}
     ,.optx_type = OPTX_TYPE_SPECIAL}
  , {{"info", no_argument, NULL, MAS_CLI_OPT_INFO}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_INFO}
  , {{"quit", no_argument, NULL, MAS_CLI_OPT_QUIT}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_QUIT}
  , {{"exitsleep", optional_argument, NULL, MAS_CLI_OPT_EXITSLEEP}
     ,.optx_type = OPTX_TYPE_UNSIGNED,.def = 29,.shift = offsetof( mas_options_t, exitsleep )}
  , {{"single", no_argument, NULL, MAS_CLI_OPT_SINGLE_INSTANCE}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_SINGLE_INSTANCE}
  , {{"single-instance", no_argument, NULL, MAS_CLI_OPT_SINGLE_INSTANCE}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_SINGLE_INSTANCE}
  , {{"multi-instance", no_argument, NULL, MAS_CLI_OPT_NOSINGLE_INSTANCE}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_SINGLE_INSTANCE}
  , {{"single-child", no_argument, NULL, MAS_CLI_OPT_SINGLE_CHILD}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_SINGLE_CHILD}
  , {{"multi-child", no_argument, NULL, MAS_CLI_OPT_NOSINGLE_CHILD}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_SINGLE_CHILD}

  , {{"save-user-opts-file", optional_argument, NULL, MAS_CLI_OPT_SAVE_USER_OPTS_FILE}
     ,.optx_type = OPTX_TYPE_STR,.shift = offsetof( mas_options_t, save_user_opts_filename )}

  , {{"save-user-opts", no_argument, NULL, MAS_CLI_OPT_SAVE_USER_OPTS}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_SAVE_USER_OPTS}
  , {{"nosave-user-opts", no_argument, NULL, MAS_CLI_OPT_NOSAVE_USER_OPTS}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_SAVE_USER_OPTS}

  , {{"command", required_argument, NULL, MAS_CLI_OPT_COMMAND}
     ,.optx_type = OPTX_TYPE_ARGV,.shift = offsetof( mas_options_t, ctrl_commandsv )}
  , {{"redirect-messages", required_argument, NULL, MAS_CLI_OPT_MSGTO}
     ,.optx_type = OPTX_TYPE_STR,.shift = offsetof( mas_options_t, msgfilename )}
  , {{"redirect-stderr", required_argument, NULL, MAS_CLI_OPT_STDERRTO}
     ,.optx_type = OPTX_TYPE_STR,.shift = offsetof( mas_options_t, stderr_filename )}
  , {{"redirect-stdout", required_argument, NULL, MAS_CLI_OPT_STDOUTTO}
     ,.optx_type = OPTX_TYPE_STR,.shift = offsetof( mas_options_t, stdout_filename )}
  , {{"listener-single", no_argument, NULL, MAS_CLI_OPT_LISTENER_SINGLE}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_LISTENER_SINGLE}
  , {{"transaction-single", no_argument, NULL, MAS_CLI_OPT_TRANSACTION_SINGLE}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_TRANSACTION_SINGLE}

  , {{"parent-messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES_PARENT}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_MESSAGES_PARENT}
  , {{"noparent-messages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES_PARENT}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_MESSAGES_PARENT}

  , {{"child-messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES_CHILD}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_MESSAGES_CHILD}
  , {{"nochild-messages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES_CHILD}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_MESSAGES_CHILD}

  , {{"messages", no_argument, NULL, MAS_CLI_OPT_MESSAGES}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_MESSAGES}
  , {{"nomessages", no_argument, NULL, MAS_CLI_OPT_NOMESSAGES}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_MESSAGES}

  , {{"redirect-std", no_argument, NULL, MAS_CLI_OPT_NOWOREDIRECT_STD}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_DAEMON_DISABLE_REDIRECT_STD}
  , {{"noredirect-std", no_argument, NULL, MAS_CLI_OPT_WOREDIRECT_STD}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_DAEMON_DISABLE_REDIRECT_STD}

  , {{"close-std", no_argument, NULL, MAS_CLI_OPT_NOWOCLOSE_STD}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_DAEMON_DISABLE_CLOSE_STD}
  , {{"noclose-std", no_argument, NULL, MAS_CLI_OPT_WOCLOSE_STD}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_DAEMON_DISABLE_CLOSE_STD}

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
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_READ_USER_OPTS}
  , {{"noread-user-config", no_argument, NULL, MAS_CLI_OPT_NOREAD_USER_OPTS}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_READ_USER_OPTS}

  , {{"read-user-config-plus", no_argument, NULL, MAS_CLI_OPT_READ_USER_OPTS_PLUS}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_READ_USER_OPTS_PLUS}
  , {{"noread-user-config-plus", no_argument, NULL, MAS_CLI_OPT_NOREAD_USER_OPTS_PLUS}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_READ_USER_OPTS_PLUS}

  , {{"overwrite-user-config", no_argument, NULL, MAS_CLI_OPT_OVERWRITE_USER_OPTS}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_OVERWRITE_USER_OPTS}
  , {{"nooverwrite-user-config", no_argument, NULL, MAS_CLI_OPT_NOOVERWRITE_USER_OPTS}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_OVERWRITE_USER_OPTS}

  , {{"overwrite-user-config-plus", no_argument, NULL, MAS_CLI_OPT_OVERWRITE_USER_OPTS_PLUS}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_OVERWRITE_USER_OPTS_PLUS}
  , {{"nooverwrite-user-config-plus", no_argument, NULL, MAS_CLI_OPT_NOOVERWRITE_USER_OPTS_PLUS}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_OVERWRITE_USER_OPTS_PLUS}

  , {{"watcher", no_argument, NULL, MAS_CLI_OPT_NOWOWATCHER}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_WATCHER}
  , {{"nowatcher", no_argument, NULL, MAS_CLI_OPT_WOWATCHER}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_WATCHER}

  , {{"ticker", no_argument, NULL, MAS_CLI_OPT_NOWOTICKER}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_WOTICKER}
  , {{"noticker", no_argument, NULL, MAS_CLI_OPT_WOTICKER}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_WOTICKER}

  , {{"ticker-mode", required_argument, NULL, MAS_CLI_OPT_TICKER_MODE}
     ,.optx_type = OPTX_TYPE_UNSIGNED,.shift = offsetof( mas_options_t, ticker_mode )}

  , {{"pidfile", no_argument, NULL, MAS_CLI_OPT_PIDFILE}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_PIDFILE}
  , {{"nopidfile", no_argument, NULL, MAS_CLI_OPT_NOPIDFILE}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_PIDFILE}

  , {{"master", no_argument, NULL, MAS_CLI_OPT_NOWOMASTER}
     ,.optx_type = OPTX_TYPE_INT,.shift = offsetof( mas_options_t, womaster )}
  , {{"nomaster", required_argument, NULL, MAS_CLI_OPT_WOMASTER}
     ,.optx_type = OPTX_TYPE_INT,.shift = offsetof( mas_options_t, womaster ),.def = 28}

  , {{"mthread", no_argument, NULL, MAS_CLI_OPT_MASTER_THREAD}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_MAKE_MASTER_THREAD}
  , {{"nomthread", no_argument, NULL, MAS_CLI_OPT_NOMASTER_THREAD}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_MAKE_MASTER_THREAD}

  , {{"listener", no_argument, NULL, MAS_CLI_OPT_NOWOLISTENER}
     ,.optx_type = OPTX_TYPE_INT,.shift = offsetof( mas_options_t, wolistener )}
  , {{"nolistener", required_argument, NULL, MAS_CLI_OPT_WOLISTENER}
     ,.optx_type = OPTX_TYPE_INT,.def = 31,.shift = offsetof( mas_options_t, wolistener ),.def = 27}

  , {{"listen", no_argument, NULL, MAS_CLI_OPT_NOWOLISTEN}
     ,.optx_type = OPTX_TYPE_INT,.shift = offsetof( mas_options_t, wolisten )}
  , {{"nolisten", required_argument, NULL, MAS_CLI_OPT_WOLISTEN}
     ,.optx_type = OPTX_TYPE_INT,.shift = offsetof( mas_options_t, wolisten )}

  , {{"daemon", no_argument, NULL, MAS_CLI_OPT_NOWODAEMON}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_DAEMON_DISABLE}
  , {{"nodaemon", no_argument, NULL, MAS_CLI_OPT_WODAEMON}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_DAEMON_DISABLE}

  , {{"sysdaemon", no_argument, NULL, MAS_CLI_OPT_SYSDAEMON}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_DAEMON_SYS}
  , {{"nosysdaemon", no_argument, NULL, MAS_CLI_OPT_NOSYSDAEMON}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_DAEMON_SYS}

  , {{"sys-daemon", no_argument, NULL, MAS_CLI_OPT_SYSDAEMON}
     ,.optx_type = OPTX_TYPE_FLAG,.shift = MAS_OPT_BITNUM_DAEMON_SYS}
  , {{"nosys-daemon", no_argument, NULL, MAS_CLI_OPT_NOSYSDAEMON}
     ,.optx_type = OPTX_TYPE_NOFLAG,.shift = MAS_OPT_BITNUM_DAEMON_SYS}

  , {{"proto", required_argument, NULL, MAS_CLI_OPT_PROTO}
     ,.optx_type = OPTX_TYPE_ARGV,.shift = offsetof( mas_options_t, protosv )}
  , {{"noprotos", no_argument, NULL, MAS_CLI_OPT_NOPROTOS}
     ,.optx_type = OPTX_TYPE_ARGV_CLEAR,.shift = offsetof( mas_options_t, protosv )}

  , {{"host", required_argument, NULL, MAS_CLI_OPT_HOST}
     ,.optx_type = OPTX_TYPE_ARGV,.shift = offsetof( mas_options_t, hostsv )}
  , {{"nohosts", no_argument, NULL, MAS_CLI_OPT_NOHOSTS}
     ,.optx_type = OPTX_TYPE_ARGV_CLEAR,.shift = offsetof( mas_options_t, hostsv )}

  , {{"port", required_argument, NULL, MAS_CLI_OPT_PORT}
     ,.optx_type = OPTX_TYPE_UNSIGNED,.shift = offsetof( mas_options_t, default_port )}

  , {{"msg", required_argument, NULL, MAS_CLI_OPT_MSG}
     ,.optx_type = OPTX_TYPE_MSGFLAG /* ?? */ }
  , {{"nomsg", no_argument, NULL, MAS_CLI_OPT_NOMSG}
     ,.optx_type = OPTX_TYPE_NOMSGFLAG}

  , {{"init-msg", optional_argument, NULL, MAS_CLI_OPT_INIT_MSG}
     ,.optx_type = OPTX_TYPE_STR,.shift = offsetof( mas_options_t, init_message )}
  , {{"init-message", required_argument, NULL, MAS_CLI_OPT_INIT_MSG}
     ,.optx_type = OPTX_TYPE_STR,.shift = offsetof( mas_options_t, init_message )}

  , {{NULL, 0, NULL, 0}
     ,.optx_type = OPTX_TYPE_NONE}
};


/* mas_optionx_t *               */
/* mas_cli_find_optx( int opt )  */
/* {                             */
/*   mas_optionx_t *optx = NULL; */
/*                               */
/*   return optx;                */
/* }                             */


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

/* int                                                                                      */
/* mas_cli_options_data_init( mas_options_t * popts, const char **message, unsigned flags ) */
INIT_HANDLER( mas_cli_options_data_init )
{
  if ( popts )
  {
    popts->cli_longopts_num = sizeof( mas_cli_optx_table ) / sizeof( mas_cli_optx_table[0] );
    popts->cli_longopts = mas_cli_create_longopts_table( mas_cli_optx_table, popts->cli_longopts_num );
    popts->cli_optxs = &mas_cli_optx_table[0];
  }
  if ( message )
    *message = __func__;
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

void
mas_cli_print_optx_table( mas_options_t * popts )
{
  for ( int indx = 0; indx < sizeof( mas_cli_optx_table ) / sizeof( mas_cli_optx_table[0] ); indx++ )
  {
    unsigned shift = 0;

    shift = popts->cli_optxs[indx].shift;
    if ( shift )
      switch ( mas_cli_optx_table[indx].optx_type )
      {
      case OPTX_TYPE_FLAG:
        {
          unsigned val = 0;

          val = popts->flag.bits &= ~( 1UL << ( shift - 1 ) ) ? 1 : 0;
          HMSG( "%c [%s]=%u", mas_cli_optx_table[indx].set ? '*' : ' ', mas_cli_optx_table[indx].longopt.name, val );
        }
        break;
      case OPTX_TYPE_NOFLAG:
        {
          unsigned val = 0;

          val = !( popts->flag.bits &= ~( 1UL << ( shift - 1 ) ) ? 1 : 0 );
          HMSG( "%c [%s]=%u", mas_cli_optx_table[indx].set ? '*' : ' ', mas_cli_optx_table[indx].longopt.name, val );
        }
        break;
      case OPTX_TYPE_MSGFLAG:
        HMSG( "%c-[%s]", mas_cli_optx_table[indx].set ? '*' : ' ', mas_cli_optx_table[indx].longopt.name );
        break;
      case OPTX_TYPE_NOMSGFLAG:
        HMSG( "%c-[%s]", mas_cli_optx_table[indx].set ? '*' : ' ', mas_cli_optx_table[indx].longopt.name );
        break;
      case OPTX_TYPE_INT:
        {
          int *p;
          int val = 0;

          p = ( int * ) ( ( ( char * ) popts ) + shift );

          val = *p;
          HMSG( "%c [%s]=%u", mas_cli_optx_table[indx].set ? '*' : ' ', mas_cli_optx_table[indx].longopt.name, val );
        }
        break;
      case OPTX_TYPE_UNSIGNED:
        {
          unsigned *p;
          unsigned val = 0;

          p = ( unsigned * ) ( ( ( char * ) popts ) + shift );

          val = *p;
          HMSG( "%c [%s]=%u", mas_cli_optx_table[indx].set ? '*' : ' ', mas_cli_optx_table[indx].longopt.name, val );
        }
        break;
      case OPTX_TYPE_STR:
        {
          char **str = NULL;
          char *val = NULL;

          str = ( char ** ) ( ( ( char * ) popts ) + shift );
          val = str ? *str : NULL;
          /* fprintf( stderr, "@@@@@@@@@@@@@@@@ [%s]='%s'\n", mas_cli_optx_table[indx].longopt.name, str ? *str : NULL ); */
          HMSG( "%c [%s]='%s'", mas_cli_optx_table[indx].set ? '*' : ' ', mas_cli_optx_table[indx].longopt.name, val );
        }
        break;
      case OPTX_TYPE_ARGV:
        {
          char *val = NULL;
          mas_string_setv_t *setv = NULL;

          setv = ( mas_string_setv_t * ) ( ( ( char * ) popts ) + shift );
          val = mas_argv_string( setv->c, setv->v, 0 );
          HMSG( "%c-[%s]={%s}", mas_cli_optx_table[indx].set ? '*' : ' ', mas_cli_optx_table[indx].longopt.name, val );
          mas_free( val );
        }
        break;
      case OPTX_TYPE_ARGV_CLEAR:
        HMSG( "%c-[%s]", mas_cli_optx_table[indx].set ? '*' : ' ', mas_cli_optx_table[indx].longopt.name );
        break;
      case OPTX_TYPE_SPECIAL:
        HMSG( "%c-[%s]", mas_cli_optx_table[indx].set ? '*' : ' ', mas_cli_optx_table[indx].longopt.name );
        break;
      case OPTX_TYPE_NONE:
        /* HMSG( "%c[%s]", mas_cli_optx_table[indx].set?'*':' ', mas_cli_optx_table[indx].longopt.name ); */
        break;
      }
  }
}

__attribute__ ( ( constructor( 3011 ) ) )
     static void mas_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOr %s\n", __FILE__ ); */
  mas_common_constructor( IL, 1 );
}
