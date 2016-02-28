#ifndef MAS_DUF_OPTION_TYPES_H
# define MAS_DUF_OPTION_TYPES_H
# include <getopt.h>

# include "duf_base_output_types.h"                                  /* mas_base_output_flags_t */

# include "duf_fun_types.h"                                          /* duf_void_int_func_t; duf_errc_int_func_t */
# include "duf_option_source_types.h"                                /* duf_option_source_t */
/* # include "duf_config_rec_etc_types.h"                               (* duf_config_rec_etc_flags_t *) */
/* # include "duf_config_opt_types.h"                                   (* duf_config_opt_disable_flags_t; duf_config_flow_flags_t  *) */
/* # include "duf_config_act_types.h"                                   (* duf_config_act_flags_t  *) */
# include "duf_option_stage_types.h"                                 /* duf_option_stage_t ; duf_limits_stage_t; duf_extended_stageopts_t */
# include "duf_option_fun_types.h"                                   /*  *_func_t;  *_funcpair_t */
# include "duf_option_class_types.h"                                 /* duf_option_class_t */

/* struct option       */
/* {                   */
/*   const char *name; */
/*   int has_arg;      */
/*   int *flag;        */
/*   int val;          */
/* };                  */
typedef struct option duf_option_t;

typedef long duf_option_gen_code_t;

typedef enum
{
# define ENUM_WRAP(_n, ...)       DUF_OPTION_VTYPE_ ## _n __VA_ARGS__,
/* #  define ENUM_WRAP_V(_n, _v) DUF_OPTION_VTYPE_ ## _n = _v, */
# define ENUM_WRAP_V(_n, _v) ENUM_WRAP(_n, = _v)
# include "duf_option_vtype_enum.def"
# undef ENUM_WRAP
# undef ENUM_WRAP_V
} duf_option_vtype_t;

typedef enum
{
# define ENUM_WRAP(_n, ...)       DUF_OPTION_CALL_TYPE_ ## _n __VA_ARGS__,
/* #  define ENUM_WRAP_V(_n, _v) DUF_OPTION_CALL_TYPE_ ## _n = _v, */
# define ENUM_WRAP_V(_n, _v) ENUM_WRAP(_n, = _v)
# include "duf_option_call_type_enum.def"
# undef ENUM_WRAP
# undef ENUM_WRAP_V
} duf_option_call_type_t;

typedef union
{
  int i;
  unsigned u;
  unsigned long ul;
  unsigned long long ull;
} duf_anynum_t;

typedef enum
{
  DUF_OFFSET_none = 0,
/* DUF_OFFSET_config, */
/* DUF_OFFSET_ufilter, */
  DUF_OFFSET_varptr,
  DUF_OFFSET_funcptr,
} duf_offset_to_t;

/* typedef union                             */
/* {                                         */
/*   duf_config_act_flags_t act;             */
/*   mas_base_output_flags_t output;         */
/*   duf_config_rec_etc_flags_t rec_etc;     */
/*   duf_config_flow_flags_t flow;           */
/*   duf_config_opt_disable_flags_t disable; */
/*   unsigned bit;                           */
/*   unsigned short sbit;                    */
/* } duf_option_anyflag_t;                   */

typedef struct
{
  int shown;

//unsigned setit:1;
//unsigned unsetit:1;
  unsigned unset:1;
  unsigned can_no:1;
  unsigned m_hasoff:1;
# if 0
  unsigned use_stage:1;
  unsigned use_stage_mask:1;
  duf_limits_stage_t stage;
  unsigned long stage_mask;
# else
  duf_extended_stageopts_t stage_opts;
# endif
  /* duf_option_anyflag_t afl_obsolete; */
  signed /* ! */ int flag_bitnum;
  unsigned long m_offset;
  duf_offset_to_t relto;
  void *reltoptr;

  const char *help;
  duf_option_class_t oclass;
  duf_option_vtype_t vtype;
  unsigned long long vmask;
  unsigned short vsize;
  duf_option_call_type_t calltype;
  duf_option_t o;

  struct
  {
    duf_anynum_t value;
    duf_anynum_t value2;
    union
    {
      duf_errc_int_arg_funcpair_t eia;
    /* duf_int_void_funcpair_t iv; */
      duf_errc_void_funcpair_t ev;
      duf_errc_cargcv_funcpair_t a;
      duf_errc_cargv_funcpair_t aa;
      duf_errc_pargcv_funcpair_t t;
      duf_errc_n_funcpair_t n;
      duf_errc_argvn_funcpair_t tn1;
    /* duf_errc_pargcvn_funcpair_t tn2; */
      duf_errc_cargvs_funcpair_t ts1;
      duf_errc_pcargcvcs_funcpair_t ts2;
      duf_errc_csv_funcpair_t s;
      duf_errc_psv_arg_funcpair_t sas;
      duf_errc_csn_arg_funcpair_t san;
    /* duf_scan_hook2_item_pair_t hi; */
    } fdesc;
    const char *funcname;
  } call;
} duf_longval_extended_t;
typedef duf_longval_extended_t *duf_longval_pextended;

typedef struct duf_longval_extended_table_s
{
  int id;
  const char *name;
# if 0
  unsigned use_stage:1;
  unsigned use_stage_mask:1;
  duf_limits_stage_t stage;
  unsigned long stage_mask;
# else
  duf_extended_stageopts_t stage_opts;
# endif
  duf_longval_extended_t xlist[];                                    /* the last ! => «flexible array member not at end of struct» */
} duf_longval_extended_table_t;

typedef struct duf_longval_extended_vtable_s
{
  int id;
  const char *name;
  duf_extended_stageopts_t stage_opts;
  duf_longval_extended_t *xlist;
} duf_longval_extended_vtable_t;

typedef struct
{
  int soft;                                                          /* unmatched tail length */
  unsigned noo:1;
  const duf_longval_extended_vtable_t *xvtable;
  const duf_longval_extended_t *xtended;
} duf_found_extended_t;

typedef struct
{
  size_t allocated_size;
  size_t count_hard;
  size_t count_soft;
  size_t hard_index;
  size_t soft_index;

  duf_found_extended_t *xarray;
} duf_found_extended_array_t;

#endif

/*
vi: ft=c
*/
