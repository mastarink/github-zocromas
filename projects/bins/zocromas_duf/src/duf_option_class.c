#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"


/* ###################################################################### */
#include "duf_option_stage.h"
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
