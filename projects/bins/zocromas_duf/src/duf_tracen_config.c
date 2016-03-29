#include <string.h>


#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_trace.h"                                        /* duf_trace_file_c; duf_trace_output_force_color_c; duf_trace_output_no_color_c ♠ */
/* ###################################################################### */
#include "duf_tracen_config.h"
/* ###################################################################### */
#ifdef MAS_TRACING
/* const duf_config_t *duf_config4trace = NULL; */

FILE *
duf_trace_file( void )
{
  return duf_trace_file_c( duf_get_trace_config(  ) );
}

#endif
