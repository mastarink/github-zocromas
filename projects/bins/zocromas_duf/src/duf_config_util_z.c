#include <string.h>

#include <mastar/tools/mas_tools.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_config_util.h"
/* ###################################################################### */

FILE *
duf_output_file_c( const duf_config_t * cfg )
{
  return cfg && cfg->opt.output.out ? cfg->opt.output.out : stdout;
}

FILE *
duf_trace_file_c( const duf_config_opt_trace_t * cfg )
{
/* return cfg && cfg->MAST_TRACE_AT_CFG output.out ? cfg->MAST_TRACE_AT_CFG output.out : stdout; */
  return cfg && cfg->output.out ? cfg->output.out : stdout;
}

int
duf_trace_force_color_c( const duf_config_t * cfg )
{
/* fprintf(stderr, "@@@@@@@@@@@@@@FC:%d\n", cfg ? cfg->opt.act.v.flag.force_color : 0 ); */
  return cfg ? cfg->opt.act.v.flag.force_color : 0;
/* return DUF_ACTG_FLAG( force_color ); */
}

int
duf_trace_nocolor_c( const duf_config_t * cfg )
{
  return cfg ? cfg->opt.act.v.flag.nocolor : 0;
/* return DUF_ACTG_FLAG( nocolor ); */
}

