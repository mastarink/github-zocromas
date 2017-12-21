#ifndef MUCS_STRUCTS_H
# define MUCS_STRUCTS_H

# include "types.h"
# include "mulconfnt_types.h"

# define MUCS_NONOPT_NAME "@NONOPT@"

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

struct mucs_option_static_s
{
  char *name;
  char shortn;
  mucs_restype_t restype;
  void *cust_ptr;
  int val;
  char *desc;
  char *argdesc;
  const char *def_string_value;
  nvalue_t def_nvalue;
  mucs_flags_t flags;
  mucs_option_callback_t callback;
  int cb_pass;
  int v_pass;
  void *extra_data;
};

#endif
