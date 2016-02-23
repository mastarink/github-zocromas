#include <string.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_config_trace.h"
/* ###################################################################### */
#include "duf_tracen_config.h"
/* ###################################################################### */
#ifdef MAS_TRACING
const duf_config_t *duf_config4trace = NULL;

duf_config_trace_t *
duf_get_trace_config( void )
{
  assert( duf_config );
  return duf_config->opt.ptracecfg;
}

FILE *
duf_trace_file( void )
{
  return duf_trace_file_c( duf_get_trace_config(  ) );
}

#endif


