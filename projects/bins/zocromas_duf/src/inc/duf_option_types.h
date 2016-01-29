#ifndef MAS_DUF_OPTION_TYPES_H
#  define MAS_DUF_OPTION_TYPES_H
#  include <getopt.h>

#  include "duf_base_types.h"
#  include "duf_flags_types.h"
#  include "duf_fun_types.h"
#  include "duf_hook_types.h"
/* #  include "duf_options_enum.h" */

typedef enum
{
  DUF_OPTION_SOURCE_NONE = 0,
  DUF_OPTION_SOURCE_MIN,
  DUF_OPTION_SOURCE_ENV = DUF_OPTION_SOURCE_MIN,
  DUF_OPTION_SOURCE_STDIN,
  DUF_OPTION_SOURCE_STREAM,
  DUF_OPTION_SOURCE_DUFFILE,
  DUF_OPTION_SOURCE_CFG,
  DUF_OPTION_SOURCE_CFGLAST,
  DUF_OPTION_SOURCE_CFGSTG,
  DUF_OPTION_SOURCE_CLI,
  DUF_OPTION_SOURCE_INTERACTIVE,
  DUF_OPTION_SOURCE_INTERAC = DUF_OPTION_SOURCE_INTERACTIVE,
  DUF_OPTION_SOURCE_MAX = DUF_OPTION_SOURCE_INTERAC,
} duf_option_source_code_t;
typedef struct
{
  duf_option_source_code_t sourcecode;
  int labelled;
  const char *label;
} duf_option_source_t;

#  define  DUF_OPTION_SOURCE_LABELLED(_code, _label) ((duf_option_source_t){.sourcecode = DUF_OPTION_SOURCE_ ## _code, .label=_label, .labelled=1 })
#  define  DUF_OPTION_SOURCE(_code ) ((duf_option_source_t){.sourcecode = DUF_OPTION_SOURCE_ ## _code })
#  define DUF_OPTION_SOURCE_DEFAULT(_source, _code) ((duf_option_source_t){ \
    		.sourcecode = (_source.sourcecode==DUF_OPTION_SOURCE_NONE?DUF_OPTION_SOURCE_ ## _code:_source.sourcecode), \
    		.label=_source.label, \
    		.labelled=_source.labelled \
    	})
#  define DUF_IS_SOURCE(_source, _code) (_source.sourcecode==DUF_OPTION_SOURCE_ ## _code)

typedef enum
{
  DUF_OPTION_STAGE_NONE = 0,
  DUF_OPTION_STAGE_MIN = 1,
  DUF_OPTION_STAGE_BOOT = DUF_OPTION_STAGE_MIN,
  DUF_OPTION_STAGE_DEBUG,
  DUF_OPTION_STAGE_PRESETUP,
  DUF_OPTION_STAGE_PRESET = DUF_OPTION_STAGE_PRESETUP,
  DUF_OPTION_STAGE_SETUP,
  DUF_OPTION_STAGE_FIRST,
  DUF_OPTION_STAGE_LOOP,
  DUF_OPTION_STAGE_INTERACTIVE,
  DUF_OPTION_STAGE_INTERAC = DUF_OPTION_STAGE_INTERACTIVE,
  DUF_OPTION_STAGE_MAX = DUF_OPTION_STAGE_INTERACTIVE,
  DUF_OPTION_STAGE_ANY = 88888,
  DUF_OPTION_STAGE_ALL = 99999,
} duf_option_stage_t;

typedef enum
{
#  define ENUM_WRAP(_n, ...)       DUF_OPTION_CLASS_ ## _n __VA_ARGS__,
/* #  define ENUM_WRAP_V(_n, _v) DUF_OPTION_CLASS_ ## _n = _v, */
#  define ENUM_WRAP_V(_n, _v) ENUM_WRAP(_n, = _v)
#  include "duf_option_class_enum.def"
#  undef ENUM_WRAP
#  undef ENUM_WRAP_V
} duf_option_class_t;

typedef enum
{
#  define ENUM_WRAP(_n, ...)       DUF_OPTION_VTYPE_ ## _n __VA_ARGS__,
/* #  define ENUM_WRAP_V(_n, _v) DUF_OPTION_VTYPE_ ## _n = _v, */
#  define ENUM_WRAP_V(_n, _v) ENUM_WRAP(_n, = _v)
#  include "duf_option_vtype_enum.def"
#  undef ENUM_WRAP
#  undef ENUM_WRAP_V
} duf_option_vtype_t;

typedef enum
{
#  define ENUM_WRAP(_n, ...)       DUF_OPTION_CALL_TYPE_ ## _n __VA_ARGS__,
/* #  define ENUM_WRAP_V(_n, _v) DUF_OPTION_CALL_TYPE_ ## _n = _v, */
#  define ENUM_WRAP_V(_n, _v) ENUM_WRAP(_n, = _v)
#  include "duf_option_call_type_enum.def"
#  undef ENUM_WRAP
#  undef ENUM_WRAP_V
} duf_option_call_type_t;


/* struct option       */
/* {                   */
/*   const char *name; */
/*   int has_arg;      */
/*   int *flag;        */
/*   int val;          */
/* };                  */
typedef struct option duf_option_t;

/********************************************************************************************/
typedef struct
{
  duf_void_int_func_t func;
  int arg;
} duf_void_int_arg_funcpair_t;
typedef struct
{
  duf_errc_int_func_t func;
  int arg;
} duf_errc_int_arg_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_int_void_func_t func;
} duf_int_void_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_cargcv_func_t func;
} duf_void_cargcv_funcpair_t;
typedef struct
{
  duf_errc_cargcv_func_t func;
} duf_errc_cargcv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_carg_func_t func;
} duf_void_carg_funcpair_t;
typedef struct
{
  duf_errc_carg_func_t func;
} duf_errc_carg_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_pargcv_func_t func;
} duf_void_pargcv_funcpair_t;
typedef struct
{
  duf_errc_pargcv_func_t func;
} duf_errc_pargcv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_n_func_t func;
} duf_void_n_funcpair_t;
typedef struct
{
  duf_errc_n_func_t func;
} duf_errc_n_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_pargcvn_func_t func;
} duf_void_pargcvn_funcpair_t;
typedef struct
{
  duf_errc_pargcvn_func_t func;
} duf_errc_pargcvn_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_errc_argvn_func_t func;
} duf_errc_argvn_funcpair_t;

/********************************************************************************************/

typedef struct
{
  duf_void_csv_func_t func;
} duf_void_csv_funcpair_t;
typedef struct
{
  duf_errc_csv_func_t func;
} duf_errc_csv_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_csv_func_t func;
  const char *arg;
} duf_void_psv_arg_funcpair_t;
typedef struct
{
  duf_errc_csv_func_t func;
  const char *arg;
} duf_errc_psv_arg_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_csn_func_t func;
  long arg;
} duf_void_csn_arg_funcpair_t;
typedef struct
{
  duf_errc_csn_func_t func;
  long arg;
} duf_errc_csn_arg_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_void_pcargcvcs_func_t func;
} duf_void_pcargcvcs_funcpair_t;
typedef struct
{
  duf_errc_pcargcvcs_func_t func;
} duf_errc_pcargcvcs_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_errc_cargvs_func_t func;
} duf_errc_cargvs_funcpair_t;

/********************************************************************************************/

typedef struct
{
  duf_void_void_func_t func;
} duf_void_void_funcpair_t;
typedef struct
{
  duf_errc_void_func_t func;
} duf_errc_void_funcpair_t;

/********************************************************************************************/
typedef struct
{
  duf_scan_hook2_item_func_t func;
  duf_scan_callbacks_t *sccb;
} duf_scan_hook2_item_pair_t;

/********************************************************************************************/

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
  DUF_OFFSET_config,
  /* DUF_OFFSET_depthinfo, */
  DUF_OFFSET_ufilter,
} duf_offset_to_t;

typedef struct
{
  /* unsigned flag:1; ??? */
  duf_option_stage_t min;
  duf_option_stage_t max;
} duf_limits_stage_t;


typedef struct
{
  int shown;

//unsigned setit:1;
//unsigned unsetit:1;
  unsigned invert:1;
  unsigned can_no:1;
  unsigned m_hasoff:1;
  unsigned use_stage:1;
  unsigned use_stage_mask:1;
  duf_anyflag_t afl;
  unsigned long m_offset;
  duf_offset_to_t relto;

  duf_limits_stage_t stage;
  unsigned long stage_mask;
  const char *help;
  duf_option_class_t oclass;
  duf_option_vtype_t vtype;
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
      duf_errc_carg_funcpair_t aa;
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
  unsigned use_stage:1;
  unsigned use_stage_mask:1;
  int id;
  const char *name;
  duf_limits_stage_t stage;
  unsigned long stage_mask;
  duf_longval_extended_t table[];
} duf_longval_extended_table_t;



typedef int ( *duf_xclarifier_t ) ( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                                    const duf_longval_extended_table_t * xtable, int no, duf_option_source_t source );

typedef struct
{
  int soft;                     /* unmatched tail length */
  int noo;
  const duf_longval_extended_table_t *xtable;
  const duf_longval_extended_t *xtended;
} duf_found_extended_t;

typedef struct
{
  size_t allocated_size;
  size_t count_hard;
  size_t count_soft;
  size_t hard_index;
  size_t soft_index;

  duf_found_extended_t *array;
} duf_found_extended_array_t;

typedef struct
{
  unsigned string_split:1;
  /* const char *string; */
  char *string_copy;
  char vseparator;
  duf_option_source_t source;
  duf_option_stage_t stage;
  const duf_longval_extended_t *extended;

  size_t name_offset;
  char *name;
  char *optarg;
  int has_arg;

  const duf_longval_extended_table_t *xtable;
  int noo;
  duf_xclarifier_t clarifier;
  duf_found_extended_array_t xfound;
  signed long doindex;
  unsigned clarified[DUF_OPTION_STAGE_MAX + 1];
} duf_option_data_t;

typedef struct
{
  size_t size;
  size_t count;
  duf_option_data_t *pods;
  duf_option_stage_t last_stage;
  duf_option_source_t last_source;
  size_t stage_index[DUF_OPTION_STAGE_MAX + 1];
  size_t stage_count[DUF_OPTION_STAGE_MAX + 1];
  size_t source_index[DUF_OPTION_STAGE_MAX + 1][DUF_OPTION_SOURCE_MAX + 1];
  size_t source_count[DUF_OPTION_STAGE_MAX + 1][DUF_OPTION_SOURCE_MAX + 1];
} duf_option_adata_t;


#endif

/*
vi: ft=c
*/
