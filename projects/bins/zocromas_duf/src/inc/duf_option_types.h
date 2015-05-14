#ifndef MAS_DUF_OPTION_TYPES_H
#  define MAS_DUF_OPTION_TYPES_H
#  include <getopt.h>

#  include "duf_base_types.h"
#  include "duf_flags_types.h"
#  include "duf_fun_types.h"
#  include "duf_hook_types.h"

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
  DUF_OPTION_VTYPE_PNUM,
  DUF_OPTION_VTYPE_NL,
  DUF_OPTION_VTYPE_PNL,
  DUF_OPTION_VTYPE_NLL,
  DUF_OPTION_VTYPE_PNLL,

  DUF_OPTION_VTYPE_MIN,
  DUF_OPTION_VTYPE_MAX,
  DUF_OPTION_VTYPE_MINMAX,

  DUF_OPTION_VTYPE_MINLL,
  DUF_OPTION_VTYPE_MAXLL,
  DUF_OPTION_VTYPE_MINMAXLL,

  DUF_OPTION_VTYPE_FLAG,
  DUF_OPTION_VTYPE_PFLAG,
  DUF_OPTION_VTYPE_NOFLAG,
  DUF_OPTION_VTYPE_SFLAG,
  DUF_OPTION_VTYPE_PSFLAG,
  DUF_OPTION_VTYPE_NOSFLAG,

  DUF_OPTION_VTYPE_STR,
  DUF_OPTION_VTYPE_PSTR,

  DUF_OPTION_VTYPE_PDISTR,
  DUF_OPTION_VTYPE_PDINUM,
  DUF_OPTION_VTYPE_PDISCCB,

  DUF_OPTION_VTYPE_DATETIME,

  DUF_OPTION_VTYPE_MINDATETIME,
  DUF_OPTION_VTYPE_MAXDATETIME,
  DUF_OPTION_VTYPE_MINMAXDATETIME,

  DUF_OPTION_VTYPE_VI_CALL,
  DUF_OPTION_VTYPE_A_CALL,
  DUF_OPTION_VTYPE_TN_CALL,
  DUF_OPTION_VTYPE_TS_CALL,
  DUF_OPTION_VTYPE_FILE,
} duf_option_vtype_t;

typedef struct option duf_option_t;

typedef struct
{
  duf_void_int_t func;
  int arg;
} duf_vifuncpair_t;
typedef struct
{
  duf_int_void_t func;
} duf_ivfuncpair_t;
typedef struct
{
  duf_void_argcv_t func;
} duf_afuncpair_t;
typedef struct
{
  duf_void_pargcv_t func;
} duf_tfuncpair_t;
typedef struct
{
  duf_void_pargcnv_t func;
} duf_tnfuncpair_t;
typedef struct
{
  duf_void_pargcsv_t func;
} duf_tsfuncpair_t;
typedef struct
{
  duf_void_void_t func;
} duf_funcpair_t;
typedef struct
{
  duf_scan_hook2_item_t func;
  duf_scan_callbacks_t *sccb;
} duf_hipair_t;



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
  duf_anyflag_t afl;
  unsigned long m_offset;
  duf_offset_to_t relto;

  duf_limits_t stage;
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
      duf_vifuncpair_t vi;
      duf_ivfuncpair_t iv;
      duf_funcpair_t vv;
      duf_afuncpair_t a;
      duf_tfuncpair_t t;
      duf_tnfuncpair_t tn;
      duf_tsfuncpair_t ts;
      duf_hipair_t hi;
    } fdesc;
  } call;
} duf_longval_extended_t;

#endif

/*
vi: ft=c
*/
