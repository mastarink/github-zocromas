#ifndef MAS_DUF_OPTION_TYPES_H
#  define MAS_DUF_OPTION_TYPES_H
#  include <getopt.h>

#  include "duf_base_types.h"
#  include "duf_flags_types.h"
#  include "duf_fun_types.h"
#  include "duf_hook_types.h"
#  include "duf_options_enum.h"

typedef enum
{
  DUF_OPTION_SOURCE_NONE = 0,
  DUF_OPTION_SOURCE_ENV,
  DUF_OPTION_SOURCE_STDIN,
  DUF_OPTION_SOURCE_STREAM,
  DUF_OPTION_SOURCE_FILE,
  DUF_OPTION_SOURCE_CFG,
  DUF_OPTION_SOURCE_CLI,
  DUF_OPTION_SOURCE_INTERACTIVE,
  DUF_OPTION_SOURCE_MAX,
} duf_option_source_t;

typedef enum
{
  DUF_OPTION_CLASS_BAD = -2,
  DUF_OPTION_CLASS_ALL = -1,
  DUF_OPTION_CLASS_ANY,

  DUF_OPTION_CLASS_MIN,

  DUF_OPTION_CLASS_NONE,
  DUF_OPTION_CLASS_NO_HELP,

  DUF_OPTION_CLASS_HELP,
  DUF_OPTION_CLASS_SYSTEM,
  DUF_OPTION_CLASS_CONTROL,
  DUF_OPTION_CLASS_REFERENCE,
  DUF_OPTION_CLASS_COLLECT,
  DUF_OPTION_CLASS_SCAN,
  DUF_OPTION_CLASS_FILTER,
  DUF_OPTION_CLASS_UPDATE,
  DUF_OPTION_CLASS_REQUEST,
  DUF_OPTION_CLASS_PRINT,

  DUF_OPTION_CLASS_TRACE,
  DUF_OPTION_CLASS_DEBUG,
  DUF_OPTION_CLASS_OBSOLETE,
  DUF_OPTION_CLASS_NODESC,
  DUF_OPTION_CLASS_OTHER,

  DUF_OPTION_CLASS_MAX,
} duf_option_class_t;

typedef enum
{
  DUF_OPTION_VTYPE_NONE,
  DUF_OPTION_VTYPE_NOOP,
  DUF_OPTION_VTYPE_UPLUS,
  DUF_OPTION_VTYPE_NUM,
  DUF_OPTION_VTYPE_NUML,
  DUF_OPTION_VTYPE_NUMLL,

  DUF_OPTION_VTYPE_MIN,
  DUF_OPTION_VTYPE_MAX,
  DUF_OPTION_VTYPE_MINMAX,

  DUF_OPTION_VTYPE_MINLL,
  DUF_OPTION_VTYPE_MAXLL,
  DUF_OPTION_VTYPE_MINMAXLL,

  DUF_OPTION_VTYPE_FLAG,
  /* DUF_OPTION_VTYPE_PFLAG, */
  DUF_OPTION_VTYPE_NOFLAG,
  DUF_OPTION_VTYPE_SFLAG,
  DUF_OPTION_VTYPE_NOSFLAG,
  /* DUF_OPTION_VTYPE_PSFLAG, */

  DUF_OPTION_VTYPE_XCHR,
  DUF_OPTION_VTYPE_STR,
  DUF_OPTION_VTYPE_CSTR,
  /* DUF_OPTION_VTYPE_PSTR, */
  DUF_OPTION_VTYPE_ARGV,
  /* DUF_OPTION_VTYPE_PAA, */
  DUF_OPTION_VTYPE_TDB,
  DUF_OPTION_VTYPE_DATETIME,

  DUF_OPTION_VTYPE_MINDATETIME,
  DUF_OPTION_VTYPE_MAXDATETIME,
  DUF_OPTION_VTYPE_MINMAXDATETIME,
/******************************************/
  DUF_OPTION_VTYPE_EIA_CALL,
  DUF_OPTION_VTYPE_EV_CALL,
  DUF_OPTION_VTYPE_A_CALL,
  DUF_OPTION_VTYPE_AA_CALL,
  DUF_OPTION_VTYPE_N_CALL,
  DUF_OPTION_VTYPE_TN_CALL,
  DUF_OPTION_VTYPE_S_CALL,
  DUF_OPTION_VTYPE_SAS_CALL,
  DUF_OPTION_VTYPE_SAN_CALL,
  DUF_OPTION_VTYPE_TS_CALL,
  DUF_OPTION_VTYPE_FILE,
} duf_option_vtype_t;

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
  duf_void_argcv_func_t func;
} duf_void_argcv_funcpair_t;
typedef struct
{
  duf_errc_argcv_func_t func;
} duf_errc_argcv_funcpair_t;
/********************************************************************************************/
typedef struct
{
  duf_void_arg_func_t func;
} duf_void_arg_funcpair_t;
typedef struct
{
  duf_errc_arg_func_t func;
} duf_errc_arg_funcpair_t;
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
  duf_void_pnv_func_t func;
} duf_void_pnv_funcpair_t;
typedef struct
{
  duf_errc_pnv_func_t func;
} duf_errc_pnv_funcpair_t;
/********************************************************************************************/
typedef struct
{
  duf_void_pargcnv_func_t func;
} duf_void_pargcnv_funcpair_t;
typedef struct
{
  duf_errc_pargcnv_func_t func;
} duf_errc_pargcnv_funcpair_t;
/********************************************************************************************/
typedef struct
{
  duf_void_psv_func_t func;
} duf_void_psv_funcpair_t;
typedef struct
{
  duf_errc_psv_func_t func;
} duf_errc_psv_funcpair_t;
/********************************************************************************************/
typedef struct
{
  duf_void_psv_func_t func;
  const char *arg;
} duf_void_psv_arg_funcpair_t;
typedef struct
{
  duf_errc_psv_func_t func;
  const char *arg;
} duf_errc_psv_arg_funcpair_t;
/********************************************************************************************/
typedef struct
{
  duf_void_psnv_func_t func;
  long arg;
} duf_void_psnv_arg_funcpair_t;
typedef struct
{
  duf_errc_psnv_func_t func;
  long arg;
} duf_errc_psnv_arg_funcpair_t;
/********************************************************************************************/
typedef struct
{
  duf_void_pargcsv_func_t func;
} duf_void_pargcsv_funcpair_t;
typedef struct
{
  duf_errc_pargcsv_func_t func;
} duf_errc_pargcsv_funcpair_t;
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
  DUF_OFFSET_config = 0,
  DUF_OFFSET_depthinfo,
  DUF_OFFSET_ufilter,
} duf_offset_to_t;
typedef struct
{
  int shown;

//unsigned setit:1;
//unsigned unsetit:1;
  unsigned invert:1;
  unsigned m_hasoff:1;
  unsigned use_stage:1;
  unsigned use_stage_mask:1;
  duf_anyflag_t afl;
  unsigned long m_offset;
  duf_offset_to_t relto;

  duf_limits_t stage;
  unsigned long stage_mask;
  const char *help;
  duf_option_class_t oclass;
  duf_option_vtype_t vtype;
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
      duf_errc_argcv_funcpair_t a;
      duf_errc_arg_funcpair_t aa;
      duf_errc_pargcv_funcpair_t t;
      duf_errc_pnv_funcpair_t n;
      duf_errc_pargcnv_funcpair_t tn;
      duf_errc_pargcsv_funcpair_t ts;
      duf_errc_psv_funcpair_t s;
      duf_errc_psv_arg_funcpair_t sas;
      duf_errc_psnv_arg_funcpair_t san;
      /* duf_scan_hook2_item_pair_t hi; */
    } fdesc;
  } call;
} duf_longval_extended_t;
typedef duf_longval_extended_t *duf_longval_pextended;

typedef struct
{
  unsigned use_stage:1;
  unsigned use_stage_mask:1;
  int id;
  duf_limits_t stage;
  unsigned long stage_mask;
  duf_longval_extended_t table[];
} duf_longval_extended_table_t;

typedef enum
{
  DUF_OPTION_STAGE_ANY = -1,
  DUF_OPTION_STAGE_SETUP = 0,
  DUF_OPTION_STAGE_FIRST,
  DUF_OPTION_STAGE_LOOP,
  DUF_OPTION_STAGE_INTERACTIVE,
  DUF_OPTION_STAGE_MAX
} duf_option_stage_t;

typedef struct
{
  duf_config_act_flags_combo_t test;
  const char *name;
  duf_option_code_t id;
} duf_chk_act_flags_t;


#endif

/*
vi: ft=c
*/
