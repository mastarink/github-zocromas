#include <string.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_config_util.h"
#include "duf_config_trace.h"
/* ###################################################################### */
#include "duf_tracen_config.h"
/* ###################################################################### */
#ifdef MAS_TRACING
const duf_config_t *duf_config4trace = NULL;

FILE *
duf_trace_file( void )
{
  return duf_trace_file_c( duf_get_trace_config(  ) );
}

#endif


