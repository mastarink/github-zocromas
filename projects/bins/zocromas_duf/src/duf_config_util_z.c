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
duf_trace_file_c( const duf_config_opt_trace_t * tcfg )
{
/* return tcfg && tcfg->MAST_TRACE_AT_CFG output.out ? tcfg->MAST_TRACE_AT_CFG output.out : stdout; */
  return tcfg && tcfg->output.out ? tcfg->output.out : stdout;
}

int
duf_output_force_color_c( const duf_config_t * cfg )
{
  return cfg ? cfg->opt.output.v.flag.force_color : 0;
/* return DUF_ACTG_FLAG( force_color ); */
}

int
duf_output_nocolor_c( const duf_config_t * cfg )
{
  return cfg ? cfg->opt.output.v.flag.nocolor : 0;
/* return DUF_ACTG_FLAG( nocolor ); */
}

int
duf_output_progress_c( const duf_config_t * cfg )
{
  return cfg ? cfg->opt.output.v.flag.progress : 0;
/* return DUF_ACTG_FLAG( nocolor ); */
}
int
duf_output_use_binformat_c( const duf_config_t * cfg )
{
  return cfg ? cfg->opt.output.v.flag.use_binformat : 0;
/* return DUF_ACTG_FLAG( nocolor ); */
}
