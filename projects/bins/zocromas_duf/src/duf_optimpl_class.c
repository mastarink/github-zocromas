/* #undef MAS_TRACING */

#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/trace/mas_trace.h> */

/* ###################################################################### */
#include "duf_optimpl_class.h"
/* ###################################################################### */

const char *
duf_optclass_name( duf_optimpl_class_t oclass )
{
#define ENUM_WRAP(_name) [DUF_OPTIMPL_CLASS_ ## _name]= #_name,
#define ENUM_WRAP_V(_name, _val)
  static char *tail[] = {
#include "duf_optimpl_class_enum.def"
    NULL
  };
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  return ( oclass >= 0 && oclass < DUF_OPTIMPL_CLASS_MAX ) ? ( const char * ) /* mas_strtolower */ ( tail[oclass] ) : ( const char * ) NULL;
}

/* FIXME: duplicate function!! duf_optclass_name and duf_optclass2string */
const char *
duf_optclass2string( duf_optimpl_class_t oclass )
{
  const char *rs = NULL;

  switch ( oclass )
  {
#define ENUM_WRAP(_n)       case DUF_OPTIMPL_CLASS_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTIMPL_CLASS_ ## _n: rs= #_n; break;
#include "duf_optimpl_class_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}
