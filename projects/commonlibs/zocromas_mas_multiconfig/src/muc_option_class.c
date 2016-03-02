/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
/* #include "muc_tracen_defs_preset.h" */

#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/trace/mas_trace.h> */

/* ###################################################################### */
#include "muc_option_class.h"
/* ###################################################################### */

const char *
muc_optclass_name( muc_option_class_t oclass )
{
#define ENUM_WRAP(_name) [MUC_OPTION_CLASS_ ## _name]= #_name,
#define ENUM_WRAP_V(_name, _val)
  static char *tail[] = {
#include "muc_option_class_enum.def"
    NULL
  };
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  return ( oclass >= 0 && oclass < MUC_OPTION_CLASS_MAX ) ? ( const char * ) /* mas_strtolower */ ( tail[oclass] ) : ( const char * ) NULL;
}

/* FIXME: duplicate function!! muc_optclass_name and muc_optclass2string */
const char *
muc_optclass2string( muc_option_class_t oclass )
{
  const char *rs = NULL;

  switch ( oclass )
  {
#define ENUM_WRAP(_n)       case MUC_OPTION_CLASS_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case MUC_OPTION_CLASS_ ## _n: rs= #_n; break;
#include "muc_option_class_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}
