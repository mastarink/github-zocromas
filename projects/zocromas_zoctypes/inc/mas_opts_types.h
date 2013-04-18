#ifndef MAS_OPTS_TYPES_H
#  define MAS_OPTS_TYPES_H

#  include <stdio.h>
#  include <mastar/types/mas_common_types.h>


typedef int ( *mas_new_section_func_t ) ( const char *section );
typedef int ( *mas_at_section_func_t ) ( const char *section, const char *s );
typedef int ( *mas_unknown_opt_func_t ) ( const char *s );
typedef void ( *mas_opts_func_t ) ( const void *env, const char *section, const char *sectval, const char *name, const char *value );

typedef union mas_msg_options_u
{
  unsigned long word;
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
  } bit;
} mas_msg_options_t;
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
struct mas_options_s
{
  mas_msg_options_t f;
  unsigned single_instance:1;
  unsigned single_child:1;
  unsigned listener_single:1;
  unsigned transaction_single:1;
  unsigned nomessages:1;

  unsigned nodaemon:1;
  unsigned noclose_std:1;
  unsigned noredirect_std:1;

  unsigned nologger:1;
  unsigned nolog:1;
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
  char *msgfilename;
  char *configfilename;
  char *saved_configfilename;
  unsigned max_config_backup;
  double restart_sleep;
  mas_new_section_func_t new_section_func;
  mas_at_section_func_t at_section_func;
  mas_unknown_opt_func_t unknown_opt_func;
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
};
typedef struct mas_option_parse_s mas_option_parse_t;


#endif
