#ifndef MAS_CLI_OPTS_DEF_H
#  define MAS_CLI_OPTS_DEF_H

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
  MAS_CLI_OPT_INIT_MSG,
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
  MAS_CLI_OPT_PIDFILE,
  MAS_CLI_OPT_NOPIDFILE,
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


#endif
