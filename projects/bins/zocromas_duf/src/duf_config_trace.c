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

FILE *
duf_trace_file( void )
{
  return duf_trace_file_c( &duf_config->opt.trace );
}

int
duf_trace_force_color( void )
{
  return duf_trace_force_color_c( duf_config );
}

int
duf_trace_nocolor( void )
{
  return duf_trace_nocolor_c( duf_config );
}

int
duf_verbose( void )
{
  return duf_config ? duf_config->opt.dbg.verbose : 0;
}
