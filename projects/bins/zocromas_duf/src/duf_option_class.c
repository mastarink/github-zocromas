/* #undef MAS_TRACING */
#include <stddef.h>                                                  /* NULL */

/* ###################################################################### */
#include "duf_option_class.h"
/* ###################################################################### */

const char *
duf_optclass_name( duf_option_class_t oclass )
{
#define ENUM_WRAP(_name) [DUF_OPTION_CLASS_ ## _name]= #_name,
#define ENUM_WRAP_V(_name, _val)
  static char *tail[] = {
#include "duf_option_class_enum.def"
    NULL
  };
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  return ( oclass >= 0 && oclass < DUF_OPTION_CLASS_MAX ) ? ( const char * ) /* mas_strtolower */ ( tail[oclass] ) : ( const char * ) NULL;
}

/* FIXME: duplicate function!! duf_optclass_name and duf_optclass2string */
const char *
duf_optclass2string( duf_option_class_t oclass )
{
  const char *rs = NULL;

  switch ( oclass )
  {
#define ENUM_WRAP(_n)       case DUF_OPTION_CLASS_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTION_CLASS_ ## _n: rs= #_n; break;
#include "duf_option_class_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}
