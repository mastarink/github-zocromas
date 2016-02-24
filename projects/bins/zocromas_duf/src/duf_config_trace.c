#include <string.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_config_trace.h"
/* ###################################################################### */

/* TODO 20160223.182516 move to trace-lib; rename accordingly */
FILE *
duf_trace_file_c( const mas_config_trace_t * tcfg )
{
/* return tcfg && tcfg->MAST_TRACE_AT_CFG output.out ? tcfg->MAST_TRACE_AT_CFG output.out : stdout; */
  return tcfg && tcfg->stream.out ? tcfg->stream.out : stdout;
}

int
duf_trace_output_force_color_c( const mas_config_trace_t * tcfg )
{
  return tcfg && tcfg->stream.v.flag.force_color ? tcfg->stream.v.flag.force_color : 0;
}

int
duf_trace_output_no_color_c( const mas_config_trace_t * tcfg )
{
  return tcfg && tcfg->stream.v.flag.nocolor ? tcfg->stream.v.flag.nocolor : 0;
}
