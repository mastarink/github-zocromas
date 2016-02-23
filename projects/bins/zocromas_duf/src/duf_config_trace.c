#include <string.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_config_trace.h"
/* ###################################################################### */

FILE *
duf_trace_file_c( const duf_config_trace_t * tcfg )
{
/* return tcfg && tcfg->MAST_TRACE_AT_CFG output.out ? tcfg->MAST_TRACE_AT_CFG output.out : stdout; */
  return tcfg && tcfg->output.stream.out ? tcfg->output.stream.out : stdout;
}

int
duf_trace_output_force_color_c( const duf_config_trace_t * tcfg )
{
  return tcfg && tcfg->output.stream.v.flag.force_color ? tcfg->output.stream.v.flag.force_color : 0;
}

int
duf_trace_output_no_color_c( const duf_config_trace_t * tcfg )
{
  return tcfg && tcfg->output.stream.v.flag.nocolor ? tcfg->output.stream.v.flag.nocolor : 0;
}
