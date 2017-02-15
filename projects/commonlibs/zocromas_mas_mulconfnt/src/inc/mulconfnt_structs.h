#ifndef MULCONF_STRUCTS_H
# define MULCONF_STRUCTS_H

# include <mastar/tools/mas_argvc_tools.h>

# include "mulconfnt_types.h"

enum mucs_source_type_e
{
  MULCONF_SOURCE_NONE,
  MULCONF_SOURCE_STRING,
  MULCONF_SOURCE_ENV,
  MULCONF_SOURCE_ARGV,
  MULCONF_SOURCE_MARGV,
  MULCONF_SOURCE_CONFIG,
  MULCONF_SOURCE_FILE,
  MULCONF_SOURCE_STREAM,
  MULCONF_SOURCE_MAX = MULCONF_SOURCE_STREAM,
};

enum mucs_source_mode_e
{
  MULCONF_MODE_NONE,
  MULCONF_MODE_LOAD,
  MULCONF_MODE_SERIAL,
  MULCONF_MODE_INTERACTIVE,
  MULCONF_MODE_MAX = MULCONF_MODE_INTERACTIVE,
};

enum mucs_variant_e
{
  MULCONF_VARIANT_SHORT,
  MULCONF_VARIANT_LONG,
  MULCONF_VARIANT_NONOPT,
  MULCONF_VARIANT_IGNORE,
  MULCONF_VARIANT_BAD,
  MULCONF_VARIANT_MAX = MULCONF_VARIANT_BAD,
  MULCONF_VARIANTS,
};

enum mucs_restype_e
{
  MULCONF_RTYP_NONE,
  MULCONF_RTYP_STRING,
  MULCONF_RTYP_TARG,
  MULCONF_RTYP_CHAR,
  MULCONF_RTYP_UCHAR,
  MULCONF_RTYP_SHORT,
  MULCONF_RTYP_USHORT,
  MULCONF_RTYP_INT,
  MULCONF_RTYP_UINT,
  MULCONF_RTYP_LONG,
  MULCONF_RTYP_ULONG,
  MULCONF_RTYP_LONG_LONG,
  MULCONF_RTYP_LLONG = MULCONF_RTYP_LONG_LONG,
  MULCONF_RTYP_ULONG_LONG,
  MULCONF_RTYP_ULLONG = MULCONF_RTYP_ULONG_LONG,
  MULCONF_RTYP_DOUBLE,
  MULCONF_RTYP_LDOUBLE,
  MULCONF_RTYP_ALIAS,
  MULCONF_RTYP_MAX = MULCONF_RTYP_ALIAS,
};
enum mucs_bitwise_e
{
  MULCONF_RTYP_FLAG_BITWISE_AND = 0x10000000L,
  MULCONF_RTYP_FLAG_BITWISE_OR = MULCONF_RTYP_FLAG_BITWISE_AND >> 1L,
  MULCONF_RTYP_FLAG_BITWISE_XOR = MULCONF_RTYP_FLAG_BITWISE_OR >> 1L,
  MULCONF_RTYP_FLAG_BITWISE_NOT = MULCONF_RTYP_FLAG_BITWISE_XOR >> 1L,
  MULCONF_RTYP_FLAG_CALL = MULCONF_RTYP_FLAG_BITWISE_NOT >> 1L,
  MULCONF_RTYP_FLAG_NEEDVAL = MULCONF_RTYP_FLAG_CALL >> 1L,
  MULCONF_RTYP_FLAG_OPTVAL = MULCONF_RTYP_FLAG_NEEDVAL >> 1L,
  MULCONF_RTYP_FLAG_LASTOPT = MULCONF_RTYP_FLAG_OPTVAL >> 1L,
  MULCONF_RTYP_FLAG_AUTOFREE = MULCONF_RTYP_FLAG_LASTOPT >> 1L,
  MULCONF_RTYP_FLAG_ALL =
          MULCONF_RTYP_FLAG_BITWISE_AND | MULCONF_RTYP_FLAG_BITWISE_OR | MULCONF_RTYP_FLAG_BITWISE_XOR | MULCONF_RTYP_FLAG_BITWISE_NOT |
          MULCONF_RTYP_FLAG_CALL | MULCONF_RTYP_FLAG_LASTOPT | MULCONF_RTYP_FLAG_AUTOFREE,
};

enum mucs_option_flagid_e
{
  MULCONF_OPTION_FLAGID_NEED_EQ,
  MULCONF_OPTION_FLAGID_UNQUOTE,
  MULCONF_OPTION_FLAGID_SILENT,
};
enum mucs_option_flag_e
{
  MULCONF_OPTION_NEED_EQ = 1L << MULCONF_OPTION_FLAGID_NEED_EQ,
  MULCONF_OPTION_UNQUOTE = 1L << MULCONF_OPTION_FLAGID_UNQUOTE,
  MULCONF_OPTION_SILENT = 1L << MULCONF_OPTION_FLAGID_SILENT,
};

struct mucs_prefix_encoder_s
{
  char *string;
  mucs_variant_t id;
};

struct mucs_error_s
{
  int nerror;
  int line;
  const char *func;
  const char *file;
  char *msg;
};

struct mucs_source_han_s
{
  mucs_source_list_t *list;
  mucs_source_han_t *next;
  mucs_source_type_t type;
  mucs_source_mode_t mode;
  unsigned long flags;
  int count;
  int lastoptpos;
  int npos;
  const void *data_ptr;
  char delim;
  char *delims;
  char *eq;
  mucs_prefix_encoder_t pref_ids[MULCONF_VARIANTS];
  source_check_fun_t check_fun;
  source_open_fun_t open_fun;
  source_close_fun_t close_fun;
  source_load_string_fun_t load_string_fun;
  source_load_targ_fun_t load_targ_fun;

  int targ_loaded;                                                   /* sequential number of targ set */
  char *string;
  mas_argvc_t oldtarg;
  mas_argvc_t targ;
  mas_argvc_t targno;

  mucs_error_t error;
  option_callback_t callback;
  option_callback_t callbacks[MULCONF_RTYP_MAX + 1];
  size_t callback_called;
};

struct mucs_source_list_s
{
  mucs_source_han_t *first;
  mas_argvc_t targ;

  mucs_error_t error;
};

union nvalue_u
{
  char v_char;
  unsigned char v_uchar;
  short v_short;
  unsigned short v_ushort;
  int v_int;
  unsigned int v_uint;
  long v_long;
  unsigned long v_ulong;
  long long v_long_long;
  unsigned long long v_ulong_long;
  double v_double;
  long double v_ldouble;
};

struct mucs_option_han_s
{
  char *name;
  char shortname;
  mucs_restype_t restype;
  void *ptr;
  int val;
  char *desc;
  char *argdesc;
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  int has_value;
  char *string_value;
  nvalue_t nvalue;
  unsigned long flags;

  mucs_source_han_t *source;
  option_callback_t callback;
  size_t callback_called;
  int worked;
  mucs_error_t error;
};

struct mucs_option_table_list_s
{
  mucs_option_table_list_t *next;
  unsigned count;
  char *name;
  mucs_option_han_t *options;
};

#endif
