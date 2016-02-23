#include <string.h>

#include <mastar/tools/mas_tools.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_cfg_output_util.h"
/* ###################################################################### */

FILE *
duf_output_file_c( const duf_config_t * cfg )
{
  return cfg && cfg->opt.output.stream.out ? cfg->opt.output.stream.out : stdout;
}

int
duf_output_force_color_c( const duf_config_t * cfg )
{
  return cfg ? cfg->opt.output.stream.v.flag.force_color : 0;
/* return DUF_ACTG_FLAG( force_color ); */
}

int
duf_output_nocolor_c( const duf_config_t * cfg )
{
  return cfg ? cfg->opt.output.stream.v.flag.nocolor : 0;
/* return DUF_ACTG_FLAG( nocolor ); */
}

int
duf_output_progress_c( const duf_config_t * cfg )
{
  return cfg ? cfg->opt.output.stream.v.flag.progress : 0;
/* return DUF_ACTG_FLAG( nocolor ); */
}

#if 0
int
duf_output_use_binformat_c( const duf_config_t * cfg )
{
  return cfg ? cfg->opt.output.stream.v.flag.use_binformat : 0;
/* return DUF_ACTG_FLAG( nocolor ); */
}
#endif
