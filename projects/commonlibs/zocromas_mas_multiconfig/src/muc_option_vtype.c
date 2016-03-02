/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
/* #include "muc_tracen_defs_preset.h" */

#include <assert.h>                                                  /* assert */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/trace/mas_trace.h> */

/* ###################################################################### */
#include "muc_option_vtype.h"                                        /* muc_optvtype2string ♠ */
/* ###################################################################### */

const char *
muc_optvtype2string( muc_option_vtype_t ovtype )
{
  const char *rs = NULL;

  switch ( ovtype )
  {
#define ENUM_WRAP(_n)       case MUC_OPTION_VTYPE_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case MUC_OPTION_VTYPE_ ## _n: rs= #_n; break;
#include "muc_option_vtype_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}

const char *
muc_extended_call_type2string( muc_option_call_type_t call_type )
{
  const char *rs = NULL;

  switch ( call_type )
  {
#define ENUM_WRAP(_n)       case MUC_OPTION_CALL_TYPE_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case MUC_OPTION_CALL_TYPE_ ## _n: rs= #_n; break;
#include "muc_option_call_type_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}
