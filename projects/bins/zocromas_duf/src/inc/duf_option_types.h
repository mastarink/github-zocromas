#ifndef MAS_DUF_OPTION_TYPES_H
#  define MAS_DUF_OPTION_TYPES_H
#  include <getopt.h>

#  include "duf_flags_types.h"

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
  DUF_OPTION_VTYPE_UPLUS,
  DUF_OPTION_VTYPE_NUM,
  DUF_OPTION_VTYPE_NLL,
  DUF_OPTION_VTYPE_MINMAX,
  DUF_OPTION_VTYPE_MINMAXLL,
  DUF_OPTION_VTYPE_FLAG,
  DUF_OPTION_VTYPE_SFLAG,
  DUF_OPTION_VTYPE_STR,
} duf_option_vtype_t;

typedef struct option duf_option_t;


typedef struct
{
  const char *help;
  duf_option_class_t oclass;
  duf_option_vtype_t vtype;
  int shown;
  duf_option_t o;
  unsigned long m;
  unsigned mf:1;
  duf_anyflag_t afl;
} duf_longval_extended_t;

#endif
