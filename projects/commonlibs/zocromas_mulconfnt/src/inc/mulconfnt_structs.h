#ifndef MUCS_STRUCTS_H
# define MUCS_STRUCTS_H

# include <mastar/tools/mas_argvc_tools.h>

# include "mulconfnt_types.h"

enum mucs_source_type_e
{
  MUCS_SOURCE_NONE,
  MUCS_SOURCE_STRING,
  MUCS_SOURCE_ENV,
  MUCS_SOURCE_ARGV,
  MUCS_SOURCE_MARGV,
  MUCS_SOURCE_CONFIG,
  MUCS_SOURCE_FILE,
  MUCS_SOURCE_STREAM,
  MUCS_SOURCE_MAX = MUCS_SOURCE_STREAM,
};

enum mucs_source_mode_e
{
  MUCS_MODE_NONE,
  MUCS_MODE_LOAD,
  MUCS_MODE_SERIAL,
  MUCS_MODE_INTERACTIVE,
  MUCS_MODE_MAX = MUCS_MODE_INTERACTIVE,
};

enum mucs_variant_e
{
  MUCS_VARIANT_SHORT,
  MUCS_VARIANT_LONG,
  MUCS_VARIANT_NONOPT,
  MUCS_VARIANT_IGNORE,
  MUCS_VARIANT_BAD,
  MUCS_VARIANT_MAX = MUCS_VARIANT_BAD,
  MUCS_VARIANTS,
};

enum mucs_restype_e
{
  MUCS_RTYP_NONE,
  MUCS_RTYP_STRING,
  MUCS_RTYP_TARG,
  MUCS_RTYP_CHAR,
  MUCS_RTYP_UCHAR,
  MUCS_RTYP_SHORT,
  MUCS_RTYP_USHORT,
  MUCS_RTYP_INT,
  MUCS_RTYP_UINT,
  MUCS_RTYP_LONG,
  MUCS_RTYP_ULONG,
  MUCS_RTYP_LONG_LONG,
  MUCS_RTYP_LLONG = MUCS_RTYP_LONG_LONG,
  MUCS_RTYP_ULONG_LONG,
  MUCS_RTYP_ULLONG = MUCS_RTYP_ULONG_LONG,
  MUCS_RTYP_DOUBLE,
  MUCS_RTYP_LDOUBLE,
  MUCS_RTYP_ALIAS,
  MUCS_RTYP_MAX = MUCS_RTYP_ALIAS,
};
enum mucs_bitwise_e
{
  MUCS_RTYP_BW_AND = 0x10000000L,
  MUCS_RTYP_BW_OR = MUCS_RTYP_BW_AND >> 1L,
  MUCS_RTYP_BW_XOR = MUCS_RTYP_BW_OR >> 1L,
  MUCS_RTYP_BW_NOT = MUCS_RTYP_BW_XOR >> 1L,
/* MUCS_RTYP_FLAG_CALL = MUCS_RTYP_BW_NOT >> 1L, */
/* MUCS_RTYP_FLAG_NEEDVAL = MUCS_RTYP_FLAG_CALL >> 1L, */
/* MUCS_RTYP_FLAG_OPTVAL = MUCS_RTYP_FLAG_NEEDVAL >> 1L, */
/* MUCS_RTYP_FLAG_LASTOPT = MUCS_RTYP_FLAG_OPTVAL >> 1L, */
/*MUCS_RTYP_FLAG_AUTOFREE = MUCS_RTYP_FLAG_LASTOPT >> 1L, */
  MUCS_RTYP_FLAG_ALL = MUCS_RTYP_BW_AND | MUCS_RTYP_BW_OR | MUCS_RTYP_BW_XOR | MUCS_RTYP_BW_NOT
        /* | MUCS_RTYP_FLAG_CALL *//* | MUCS_RTYP_FLAG_LASTOPT *//* | MUCS_RTYP_FLAG_AUTOFREE */ ,
};

enum mucs_flag_id_e
{
  MUCS_FLAG_ID_AUTOFREE,
  MUCS_FLAG_ID_LASTOPT,
  MUCS_FLAG_ID_NEED_EQ,
  MUCS_FLAG_ID_SILENT,
  MUCS_FLAG_ID_UNQUOTE,
};
enum mucs_flag_e
{
  MUCS_FLAG_AUTOFREE = 1L << MUCS_FLAG_ID_AUTOFREE,
  MUCS_FLAG_LASTOPT = 1L << MUCS_FLAG_ID_LASTOPT,
  MUCS_FLAG_NEED_EQ = 1L << MUCS_FLAG_ID_NEED_EQ,
  MUCS_FLAG_SILENT = 1L << MUCS_FLAG_ID_SILENT,
  MUCS_FLAG_UNQUOTE = 1L << MUCS_FLAG_ID_UNQUOTE,
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
  int lastoptpos;                                                    /*number of arg with last opt */
  int curarg;                                                        /* number of arg withc current arg */
  int npos;
  const void *data_ptr;
  char delim;
  char *delims;
  char *eq;
  mucs_prefix_encoder_t pref_ids[MUCS_VARIANTS];
  mucs_source_check_fun_t check_fun;
  mucs_source_open_fun_t open_fun;
  mucs_source_close_fun_t close_fun;
  mucs_source_load_string_fun_t load_string_fun;
  mucs_source_load_targ_fun_t load_targ_fun;

  void *ptr_internal;
  int targ_loaded;                                                   /* sequential number of targ set */
  char *string;
  mas_argvc_t oldtarg;
  mas_argvc_t targ;
  mas_argvc_t targno;

  mucs_error_t error;
  mucs_option_callback_t callback;
  mucs_option_callback_t callbacks[MUCS_RTYP_MAX + 1];
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
  mucs_option_callback_t callback;
  size_t callback_called;
  int value_is_set;
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
