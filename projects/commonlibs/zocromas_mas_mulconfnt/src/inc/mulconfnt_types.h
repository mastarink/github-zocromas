#ifndef MULCONF_TYPES_H
# define MULCONF_TYPES_H

# include <mastar/tools/mas_argvc_tools.h>

typedef enum
{
  CONFIG_SOURCE_NONE,
  CONFIG_SOURCE_DIRECT,
  CONFIG_SOURCE_ENV,
  CONFIG_SOURCE_ARGV,
  CONFIG_SOURCE_CONFIG,
  CONFIG_SOURCE_FILE,
  CONFIG_SOURCE_STREAM
} config_source_t;

typedef enum
{
  CONFIG_MODE_NONE,
  CONFIG_MODE_LOAD,
  CONFIG_MODE_SERIAL,
  CONFIG_MODE_INTERACTIVE,
} config_mode_t;

typedef struct config_source_desc_s config_source_desc_t;
typedef int ( *source_check_fun_t ) ( int count, void *data_ptr, const char *delims, const char *eq, const char *lpref, const char *pref );
typedef int ( *source_open_fun_t ) ( int count, void *data_ptr, const char *delims, const char *eq, const char *lpref, const char *pref );
typedef int ( *source_close_fun_t ) ( int count, void *data_ptr, const char *delims, const char *eq, const char *lpref, const char *pref );
typedef char *( *source_load_string_fun_t ) ( config_source_desc_t * descr );
typedef mas_argvc_t( *source_load_targ_fun_t ) ( config_source_desc_t * descr, mas_argvc_t targ );
struct config_source_desc_s
{
  config_source_desc_t *next;
  config_source_t type;
  config_mode_t mode;
  int count;
  const void *data_ptr;
  char *delims;
  char *eq;
  char *lpref;
  char *pref;
  source_check_fun_t check_fun;
  source_open_fun_t open_fun;
  source_close_fun_t close_fun;
  source_load_string_fun_t load_string_fun;
  source_load_targ_fun_t load_targ_fun;

  char *string;
  mas_argvc_t targ;
};

#endif
