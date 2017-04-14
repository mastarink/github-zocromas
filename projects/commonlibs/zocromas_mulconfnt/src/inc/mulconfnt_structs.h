#ifndef MUCS_MULCONFNT_STRUCTS_H
# define MUCS_MULCONFNT_STRUCTS_H

# include <mastar/tools/mas_argvc_tools.h>

# include "mulconfnt_types.h"
# include "mulconfnt_enums.h"
# include "structs.h"

struct mucs_prefix_encoder_s
{
  char *string;
  mucs_variant_t id;
};

# if 0
struct mucs_error_s
{
  int nerror;
  int line;
  const char *func;
  const char *file;
  char *msg;
};
# endif
struct mucs_source_extra_cb_s
{
  size_t callback_called;
  const mucs_option_table_list_t *tablist;
  const mucs_source_t *source;
};
struct mucs_source_s
{
  mucs_source_list_t *list;
  mucs_source_t *next;
  mucs_source_type_t type;
  mucs_source_mode_t mode;
  mucs_flags_t flags;
  int count;
  int lastoptpos;                                                    /*number of arg with last opt */
  int curarg;                                                        /* number of arg withc current arg */
  int ngroup;
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
  int error;
  int targ_loaded;                                                   /* sequential number of targ set */
  char *string;
  mas_argvc_t oldtarg;
  mas_argvc_t targ;
  mas_argvc_t targno;

/* mucs_error_t error; */
  mucs_option_callback_t common_callback;
  mucs_option_callback_t type_callbacks[MUCS_RTYP_MAX + 1];
  mucs_source_extra_cb_t extra_cb;

  int pass;
  int min_pass;
};

struct mucs_source_list_s
{
  mucs_source_t *first;
  mas_argvc_t targ;
};

struct mucs_option_dynamic_s
{
  int has_value;
  char *string_value;
  nvalue_t nvalue;

  int npos;

  mucs_source_t *source;                                             /* not really needed, for testing only */
  mucs_source_extra_cb_t extra_cb;
  int value_is_set;
};

struct mucs_option_s
{
# if 1
  mucs_option_static_t s;
# else
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
# endif
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
# if 1
  mucs_option_dynamic_t d;
# else
  int has_value;
  char *string_value;
  nvalue_t nvalue;

  int npos;

  mucs_source_t *source;
  mucs_source_extra_cb_t extra_cb;
  int value_is_set;
# endif
};

struct mucs_option_table_list_s
{
  unsigned allocated:1;
  mucs_option_table_list_t *next;
  unsigned optcount;                                                 /* real options number */
  char *name;
/* const */ mucs_option_t *voptions;
  const mucs_option_t *coptions;
  mucs_flags_t flags;
};

struct mucs_optscanner_s
{
  const char *arg;
  const char *at_arg;
  size_t preflen;
  const mucs_source_t *source;
  mucs_variant_t variantid;
  int has_value;
  const char *string_value;
  const char *force_value;
  const char *nextarg;
  const mucs_option_t *found_topt;
  mucs_errors_t errors;

  int pass;
};

struct mucs_option_interface_s
{
  mucs_option_table_list_t *tablist;
  mucs_source_list_t *source_list;
};

#endif
