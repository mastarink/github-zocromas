#include <string.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


/* ###################################################################### */
#include "duf_config_trace.h"
/* ###################################################################### */
const duf_config_t *duf_config4trace = NULL;


duf_config_opt_trace_t *
duf_get_trace_config( void )
{
  assert( duf_config );
  return &duf_config->opt.trace;
}
