#ifndef MAS_OPTS_TYPES_H
#  define MAS_OPTS_TYPES_H

#  include <stdio.h>
#  include <getopt.h>
#  include <mastar/types/mas_common_types.h>


/* typedef int ( *mas_new_section_func_t ) ( const char *section );               */
/* typedef int ( *mas_at_section_func_t ) ( const char *section, const char *s ); */
/* typedef int ( *mas_unknown_opt_func_t ) ( const char *s );                     */
typedef void ( *mas_opts_func_t ) ( const void *env, const char *section, const char *sectval, const char *name, const char *value );

typedef enum mas_msg_options_shift_e
{
  MAS_OPT_BITNUM_MSG_NONE,
  MAS_OPT_BITNUM_MSG_NOTICE,
  MAS_OPT_BITNUM_MSG_WATCH,
  MAS_OPT_BITNUM_MSG_MAIN,
  MAS_OPT_BITNUM_MSG_IO,
  MAS_OPT_BITNUM_MSG_LISTEN,
  MAS_OPT_BITNUM_MSG_TRANSACTION,
  MAS_OPT_BITNUM_MSG_WAIT,
  MAS_OPT_BITNUM_MSG_THREAD,
  MAS_OPT_BITNUM_MSG_SIGNAL,
  MAS_OPT_BITNUM_MSG_TRACE,
  MAS_OPT_BITNUM_MSG_TRACE_MAIN,
  MAS_OPT_BITNUM_MSG_TRACE_LISTENER,
  MAS_OPT_BITNUM_MSG_TRACE_TRANSACTION,
  MAS_OPT_BITNUM_MSG_CMD,
  MAS_OPT_BITNUM_MSG_MEM,
  MAS_OPT_BITNUM_MSG_FUNLINE,
} mas_msg_options_shift_t;

typedef union mas_msg_options_u
{
  unsigned bits;
  struct mas_optflags_s
  {
    unsigned msg_notice:1;
    unsigned msg_watch:1;
    /* main */
    unsigned msg_main:1;
    /* communication */
    unsigned msg_io:1;
    /* listener */
    unsigned msg_listen:1;
    /* transaction */
    unsigned msg_transaction:1;
    /* waiter */
    unsigned msg_wait:1;
    unsigned msg_thread:1;
    /* signal */
    unsigned msg_signal:1;
    /* trace */
    unsigned msg_trace:1;
    unsigned msg_trace_main:1;
    unsigned msg_trace_listener:1;
    unsigned msg_trace_transaction:1;
    unsigned msg_cmd:1;
    unsigned msg_mem:1;
    unsigned msg_funline:1;
  } name;
} mas_options_msg_t;

typedef struct mas_dirs_s
{
  char *pids;
  char *mods;
  char *proto;
  char *log;
  char *history;
  char *post;
  char *config;
} mas_dirs_t;
typedef struct mas_thnames_s
{
  char *parent_masterm;
  char *daemon_masterm;
  char *parent_masterx;
  char *daemon_masterx;
  char *parent_masterth;
  char *daemon_masterth;
  char *parent_masterthx;
  char *daemon_masterthx;
  char *parent_bunchm;
  char *daemon_bunchm;
  char *parent_bunchx;
  char *daemon_bunchx;
  char *parent_bunchi;
  char *daemon_bunchi;
  char *listenin;
  char *listenw;
  char *listent;
  char *listenm;
  char *listen_close;
  char *listen_exit;
} mas_thnames_t;

typedef struct mas_options_daemon_s
{
  unsigned disable:1;
  unsigned sys:1;
  unsigned disable_redirect_std:1;
  unsigned disable_close_std:1;
  unsigned disable_setsid:1;
  unsigned disable_chdir:1;
} mas_options_daemon_t;

typedef struct mas_options_log_s
{
  unsigned enable:1;
  unsigned run:1;
} mas_options_log_t;

#  define OPT_QFLAG( o, x ) ( (o)->flag.name.x ? 1 : 0 )
#  define OPT_SFLAG( o, x, v ) ( (o)->flag.name.x = ((v) ? 1 : 0 ))
typedef enum mas_optionx_type_s
{
  OPTX_TYPE_NONE,
  OPTX_TYPE_FLAG,
  OPTX_TYPE_NOFLAG,
  OPTX_TYPE_INT,
  OPTX_TYPE_UNSIGNED,
  OPTX_TYPE_STR,
  OPTX_TYPE_ARGV,
  OPTX_TYPE_ARGV_CLEAR,
} mas_optionx_type_t;
typedef struct mas_optionx_s
{
  struct option longopt;
  mas_optionx_type_t optx_type;
  mas_msg_options_shift_t shift;
  unsigned long def;
  unsigned set:1;
} mas_optionx_t;
typedef enum mas_options_shift_e
{
  MAS_OPT_BITNUM_NONE,
/* 0 */
  MAS_OPT_BITNUM_TEST,
  MAS_OPT_BITNUM_QUIT,
  MAS_OPT_BITNUM_INFO,
  MAS_OPT_BITNUM_HELP,
/* 10 */
  MAS_OPT_BITNUM_PIDFILE,
  MAS_OPT_BITNUM_SINGLE_INSTANCE,
  MAS_OPT_BITNUM_SINGLE_CHILD,
  MAS_OPT_BITNUM_LISTENER_SINGLE,
/* 100 */
  MAS_OPT_BITNUM_TRANSACTION_SINGLE,
  MAS_OPT_BITNUM_MESSAGES_PARENT,
  MAS_OPT_BITNUM_MESSAGES_CHILD,
  MAS_OPT_BITNUM_MESSAGES,
/* 1000 */
  MAS_OPT_BITNUM_WATCHER,
  MAS_OPT_BITNUM_DISCONNECT_PROMPT,
  MAS_OPT_BITNUM_WAIT_SERVER,
  MAS_OPT_BITNUM_MAKE_MASTER_THREAD,
/* 10000 */
  MAS_OPT_BITNUM_WOTICKER,
  MAS_OPT_BITNUM_READ_USER_OPTS,
  MAS_OPT_BITNUM_READ_USER_OPTS_PLUS,
  MAS_OPT_BITNUM_SAVE_USER_OPTS,
/* 100000 */
  MAS_OPT_BITNUM_SAVE_USER_OPTS_PLUS,
  MAS_OPT_BITNUM_OVERWRITE_USER_OPTS,
  MAS_OPT_BITNUM_OVERWRITE_USER_OPTS_PLUS,
  MAS_OPT_BITNUM_DAEMON_DISABLE,
/* 1000000 */
  MAS_OPT_BITNUM_DAEMON_SYS,
  MAS_OPT_BITNUM_DAEMON_DISABLE_REDIRECT_STD,
  MAS_OPT_BITNUM_DAEMON_DISABLE_CLOSE_STD,
  MAS_OPT_BITNUM_DAEMON_DISABLE_SETSID,
/* 10000000 */
  MAS_OPT_BITNUM_DAEMON_DISABLE_CHDIR,
  MAS_OPT_BITNUM_LOG_ENABLE,
  MAS_OPT_BITNUM_LOG_RUN,
  MAS_OPT_BITNUM_LOG_UNDEF1,
} mas_options_shift_t;


typedef struct mas_options_s
{
  union
  {
    unsigned long bits;
    struct
    {
      unsigned test:1;
      unsigned quit:1;
      unsigned info:1;
      unsigned help:1;
      unsigned pidfile:1;
      unsigned single_instance:1;
      unsigned single_child:1;
      unsigned listener_single:1;
      unsigned transaction_single:1;
      unsigned messages_parent:1;
      unsigned messages_child:1;
      unsigned messages:1;

      unsigned wowatcher:1;
      unsigned disconnect_prompt:1;
      unsigned wait_server:1;
      unsigned make_master_thread:1;
      unsigned woticker:1;
      unsigned read_user_opts:1;
      unsigned read_user_opts_plus:1;
      unsigned save_user_opts:1;
      unsigned save_user_opts_plus:1;
      unsigned overwrite_user_opts:1;
      unsigned overwrite_user_opts_plus:1;
      /* unsigned has_init_message:1; */
      /* mas_options_daemon_t daemon; */
      unsigned daemon_disable:1;
      unsigned daemon_sys:1;
      unsigned daemon_disable_redirect_std:1;
      unsigned daemon_disable_close_std:1;
      unsigned daemon_disable_setsid:1;
      unsigned daemon_disable_chdir:1;
      /* mas_options_log_t log; */
      unsigned log_enable:1;
      unsigned log_run:1;

      mas_options_msg_t msg;
    } name;
  } flag;

  unsigned ticker_mode;
  unsigned womaster;
  unsigned wolistener;
  unsigned wolisten;
  unsigned exitsleep;
  unsigned default_port;
  char env_optsname[256];
  char env_hostname[256];

  /* unsigned argc; */
  /* char **argv;   */
  mas_string_setv_t argvv;

  /* int protos_num; */
  /* char **protos;  */
  mas_string_setv_t protosv;

  /* int hosts_num; */
  /* char **hosts;  */
  mas_string_setv_t hostsv;

  /* int commands_num; */
  /* char **commands;  */
  mas_string_setv_t commandsv;
  mas_string_setv_t ctrl_commandsv;

  unsigned client_attempts;
  char *uuid;


  mas_dirs_t dir;
  mas_thnames_t thname;
  char *user;
  char *group;
  char *save_user_opts_filename;
  char *msgfilename;
  char *init_message;
  char *stderr_filename;
  char *stdout_filename;
  char *configfilename;
  char *saved_configfilename;
  unsigned max_config_backup;
  double restart_sleep;
  /* mas_new_section_func_t new_section_func; */
  /* mas_at_section_func_t at_section_func;   */
  /* mas_unknown_opt_func_t unknown_opt_func; */
  struct option *cli_longopts;
  unsigned cli_longopts_num;
  mas_optionx_t *cli_optxs;
} mas_options_t;


typedef enum mas_optiov_value_type_e
{
  MAS_OPT_TYPE_NONE,
  MAS_OPT_TYPE_INT,
  MAS_OPT_TYPE_DOUBLE,
  MAS_OPT_TYPE_STR,
  MAS_OPT_TYPE_PSTR,
  MAS_OPT_TYPE_FUNC,
} mas_optiov_value_type_t;

struct mas_option_parse_s
{
  const char *section;
  const char *name;
  mas_optiov_value_type_t type;
  size_t offset;
  size_t size;
  mas_opts_func_t func;
  const char *dv;
};
typedef struct mas_option_parse_s mas_option_parse_t;

#  define INIT_HANDLER( hfun ) int hfun( mas_options_t * popts, const char **message, unsigned flags  )
/* typedef int ( *mas_init_fun_t ) ( mas_options_t * popts, const char **message, unsigned flags ); */
typedef INIT_HANDLER( ( *mas_init_fun_t ) );

/* #  define MAS_NOPASS_OPTS */

#endif
