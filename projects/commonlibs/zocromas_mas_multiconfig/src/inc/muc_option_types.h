#ifndef MAS_MUC_OPTION_TYPES_H
# define MAS_MUC_OPTION_TYPES_H
# include <getopt.h>

# include "muc_base_output_types.h"                                  /* mas_base_output_flags_t */

# include "muc_fun_types.h"                                          /* muc_void_int_func_t; muc_errc_int_func_t */
# include "muc_option_source_types.h"                                /* muc_option_source_t */
# include "muc_option_stage_types.h"                                 /* muc_option_stage_t ; muc_limits_stage_t; muc_extended_stageopts_t */
# include "muc_option_fun_types.h"                                   /*  *_func_t;  *_funcpair_t */
/* # include "muc_option_class_types.h"                                 (* muc_option_class_t *) */

typedef int muc_option_gen_class_t;
/* struct option       */
/* {                   */
/*   const char *name; */
/*   int has_arg;      */
/*   int *flag;        */
/*   int val;          */
/* };                  */
typedef struct option muc_option_t;

typedef int muc_option_gen_code_t;

enum muc_option_vtype_e
{
# define ENUM_WRAP(_n, ...)       MUC_OPTION_VTYPE_ ## _n __VA_ARGS__,
/* #  define ENUM_WRAP_V(_n, _v) MUC_OPTION_VTYPE_ ## _n = _v, */
# define ENUM_WRAP_V(_n, _v) ENUM_WRAP(_n, = _v)
# include "muc_option_vtype_enum.def"
# undef ENUM_WRAP
# undef ENUM_WRAP_V
};
typedef enum muc_option_vtype_e muc_option_vtype_t;

enum muc_option_call_type_e
{
# define ENUM_WRAP(_n, ...)       MUC_OPTION_CALL_TYPE_ ## _n __VA_ARGS__,
/* #  define ENUM_WRAP_V(_n, _v) MUC_OPTION_CALL_TYPE_ ## _n = _v, */
# define ENUM_WRAP_V(_n, _v) ENUM_WRAP(_n, = _v)
# include "muc_option_call_type_enum.def"
# undef ENUM_WRAP
# undef ENUM_WRAP_V
};
typedef enum muc_option_call_type_e muc_option_call_type_t;

union muc_anynum_u
{
  int i;
  unsigned u;
  unsigned long ul;
  unsigned long long ull;
};
typedef union muc_anynum_u muc_anynum_t;

enum muc_offset_to_e
{
  MUC_OFFSET_none = 0,
/* MUC_OFFSET_config, */
/* MUC_OFFSET_ufilter, */
  MUC_OFFSET_varptr,
  MUC_OFFSET_funcptr,
};
typedef enum muc_offset_to_e muc_offset_to_t;

/* typedef union                             */
/* {                                         */
/*   muc_config_act_flags_t act;             */
/*   mas_base_output_flags_t output;         */
/*   muc_config_rec_etc_flags_t rec_etc;     */
/*   muc_config_flow_flags_t flow;           */
/*   muc_config_opt_disable_flags_t disable; */
/*   unsigned bit;                           */
/*   unsigned short sbit;                    */
/* } muc_option_anyflag_t;                   */
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
  muc_limits_stage_t stage;
  unsigned long stage_mask;
# else
  muc_extended_stageopts_t stage_opts;
# endif
/* muc_option_anyflag_t afl_obsolete; */
  signed /* ! */ int flag_bitnum;
  unsigned long m_offset;
  muc_offset_to_t relto;
  void *reltoptr;

  const char *help;
  muc_option_gen_class_t oclass;
  muc_option_vtype_t vtype;
  unsigned long long vmask;
  unsigned short vsize;
  muc_option_call_type_t calltype;
  muc_option_t o;

  struct
  {
    muc_anynum_t value;
    muc_anynum_t value2;
    union
    {
      muc_errc_int_arg_funcpair_t eia;
    /* ..._int_void_funcpair_t iv; */
      muc_errc_void_funcpair_t ev;
      muc_errc_cargcv_funcpair_t a;
      muc_errc_cargv_funcpair_t aa;
      muc_errc_pargcv_funcpair_t t;
      muc_errc_n_funcpair_t n;
      muc_errc_argvn_funcpair_t tn1;
    /* ..._errc_pargcvn_funcpair_t tn2; */
      muc_errc_cargvs_funcpair_t ts1;
      muc_errc_pcargcvcs_funcpair_t ts2;
      muc_errc_csv_funcpair_t s;
      muc_errc_psv_arg_funcpair_t sas;
      muc_errc_csn_arg_funcpair_t san;
    } fdesc;
    const char *funcname;
  } call;
} muc_longval_extended_t;
typedef muc_longval_extended_t *muc_longval_pextended_t;

typedef struct muc_longval_extended_table_s
{
  int id;
  const char *name;
  muc_extended_stageopts_t stage_opts;
  muc_longval_extended_t xlist[];                                    /* the last ! => «flexible array member not at end of struct» */
} muc_longval_extended_table_t;

typedef struct muc_longval_extended_vtable_s
{
  int id;
  const char *name;
  muc_extended_stageopts_t stage_opts;
  muc_longval_extended_t *xlist;
} muc_longval_extended_vtable_t;

typedef struct
{
  int soft;                                                          /* unmatched tail length */
  unsigned noo:1;
  const muc_longval_extended_vtable_t *xvtable;
  const muc_longval_extended_t *xtended;
} muc_found_extended_t;

typedef struct
{
  size_t allocated_size;
  size_t count_hard;
  size_t count_soft;
  size_t hard_index;
  size_t soft_index;

  muc_found_extended_t *xarray;
} muc_found_extended_array_t;

#endif

/*
vi: ft=c
*/
