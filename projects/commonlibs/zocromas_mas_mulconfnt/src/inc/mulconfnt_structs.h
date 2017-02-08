#ifndef MULCONF_STRUCTS_H
# define MULCONF_STRUCTS_H

# include <mastar/tools/mas_argvc_tools.h>

# include "mulconfnt_types.h"

enum config_source_e
{
  MULCONF_SOURCE_NONE,
  MULCONF_SOURCE_DIRECT,
  MULCONF_SOURCE_ENV,
  MULCONF_SOURCE_ARGV,
  MULCONF_SOURCE_CONFIG,
  MULCONF_SOURCE_FILE,
  MULCONF_SOURCE_STREAM,
  MULCONF_SOURCE_MAX = MULCONF_SOURCE_STREAM,
};

enum config_source_mode_e
{
  MULCONF_MODE_NONE,
  MULCONF_MODE_LOAD,
  MULCONF_MODE_SERIAL,
  MULCONF_MODE_INTERACTIVE,
  MULCONF_MODE_MAX = MULCONF_MODE_INTERACTIVE,
};

enum config_variant_e
{
  MULCONF_VARIANT_SHORT,
  MULCONF_VARIANT_LONG,
  MULCONF_VARIANT_NONOPT,
  MULCONF_VARIANT_MAX = MULCONF_VARIANT_NONOPT,
  MULCONF_VARIANTS,
  MULCONF_VARIANT_BAD,
};
struct config_prefix_encoder_s
{
  char *string;
  config_variant_t id;
};
struct config_source_desc_s
{
  config_source_desc_t *next;
  config_source_t type;
  config_source_mode_t mode;
  int count;
  const void *data_ptr;
  char delim;
  char *delims;
  char *eq;
  config_prefix_encoder_t pref_ids[MULCONF_VARIANTS];
  source_check_fun_t check_fun;
  source_open_fun_t open_fun;
  source_close_fun_t close_fun;
  source_load_string_fun_t load_string_fun;
  source_load_targ_fun_t load_targ_fun;

  char *string;
  mas_argvc_t targ;
};

struct config_source_list_s
{
  config_source_desc_t *first;
  mas_argvc_t targ;
};

enum config_restype_e
{
  MULCONF_RESTYPE_NONE,
  MULCONF_RESTYPE_STRING,
  MULCONF_RESTYPE_INT,
  MULCONF_RESTYPE_LONG,
  MULCONF_RESTYPE_MASK,
  MULCONF_RESTYPE_FUNCTION,
  MULCONF_RESTYPE_MAX = MULCONF_RESTYPE_FUNCTION,
};

struct config_option_s
{
  char *name;
  char shortname;
  config_restype_t opttype;
  void *ptr;
  int val;
  char *desc;
  char *argdesc;
};

struct config_option_table_list_s
{
  config_option_table_list_t *next;
  unsigned count;
  char *name;
  config_option_t *options;
};

#endif
