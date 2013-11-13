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
  MAS_OPT_SHIFT_MSG_NOTICE,
  MAS_OPT_SHIFT_MSG_WATCH,
  MAS_OPT_SHIFT_MSG_MAIN,
  MAS_OPT_SHIFT_MSG_IO,
  MAS_OPT_SHIFT_MSG_LISTEN,
  MAS_OPT_SHIFT_MSG_TRANSACTION,
  MAS_OPT_SHIFT_MSG_WAIT,
  MAS_OPT_SHIFT_MSG_THREAD,
  MAS_OPT_SHIFT_MSG_SIGNAL,
  MAS_OPT_SHIFT_MSG_TRACE,
  MAS_OPT_SHIFT_MSG_TRACE_MAIN,
  MAS_OPT_SHIFT_MSG_TRACE_LISTENER,
  MAS_OPT_SHIFT_MSG_TRACE_TRANSACTION,
  MAS_OPT_SHIFT_MSG_CMD,
  MAS_OPT_SHIFT_MSG_MEM,
  MAS_OPT_SHIFT_MSG_FUNLINE,
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
  OPTX_TYPE_SPECIAL,
} mas_optionx_type_t;
typedef struct mas_optionx_s
{
  struct option longopt;
  mas_optionx_type_t optx_type;
  mas_msg_options_shift_t flag_shift;
} mas_optionx_t;
typedef enum mas_options_shift_e
{
  MAS_OPT_SHIFT_TEST,
  MAS_OPT_SHIFT_QUIT,
  MAS_OPT_SHIFT_INFO,
  MAS_OPT_SHIFT_NOPIDFILE,
  MAS_OPT_SHIFT_SINGLE_INSTANCE,
  MAS_OPT_SHIFT_SINGLE_CHILD,
  MAS_OPT_SHIFT_LISTENER_SINGLE,
  MAS_OPT_SHIFT_TRANSACTION_SINGLE,
  MAS_OPT_SHIFT_NOMESSAGES_PARENT,
  MAS_OPT_SHIFT_NOMESSAGES_CHILD,
  MAS_OPT_SHIFT_NOMESSAGES,

  MAS_OPT_SHIFT_NOWATCHER,
  MAS_OPT_SHIFT_DISCONNECT_PROMPT,
  MAS_OPT_SHIFT_WAIT_SERVER,
  MAS_OPT_SHIFT_MAKE_MASTER_THREAD,
  MAS_OPT_SHIFT_NOTICKER,
  MAS_OPT_SHIFT_READ_USER_OPTS,
  MAS_OPT_SHIFT_READ_USER_OPTS_PLUS,
  MAS_OPT_SHIFT_SAVE_USER_OPTS,
  MAS_OPT_SHIFT_SAVE_USER_OPTS_PLUS,
  MAS_OPT_SHIFT_OVERWRITE_USER_OPTS,
  MAS_OPT_SHIFT_OVERWRITE_USER_OPTS_PLUS,
} mas_options_shift_t;


struct mas_options_s
{
  union
  {
    unsigned long bits;
    struct
    {
      unsigned test:1;
      unsigned quit:1;
      unsigned info:1;
      unsigned nopidfile:1;
      unsigned single_instance:1;
      unsigned single_child:1;
      unsigned listener_single:1;
      unsigned transaction_single:1;
      unsigned nomessages_parent:1;
      unsigned nomessages_child:1;
      unsigned nomessages:1;

      unsigned nowatcher:1;
      unsigned disconnect_prompt:1;
      unsigned wait_server:1;
      unsigned make_master_thread:1;
      unsigned noticker:1;
      unsigned read_user_opts:1;
      unsigned read_user_opts_plus:1;
      unsigned save_user_opts:1;
      unsigned save_user_opts_plus:1;
      unsigned overwrite_user_opts:1;
      unsigned overwrite_user_opts_plus:1;
      /* unsigned has_init_message:1; */
      mas_options_daemon_t daemon;
      mas_options_msg_t msg;
      mas_options_log_t log;
    } name;
  } flag;

  unsigned ticker_mode;
  unsigned nomaster;
  unsigned nolistener;
  unsigned nolisten;
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
};
typedef struct mas_options_s mas_options_t;


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
typedef int ( *mas_init_fun_t ) ( mas_options_t * popts, const char **message );

/* #  define MAS_NOPASS_OPTS */

#endif
