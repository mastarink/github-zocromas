#include <string.h>

/* #include <mastar/tools/mas_tools.h> */

#include "duf_config_structs.h"
/* ###################################################################### */
#include "duf_cfg_output_util.h"
/* ###################################################################### */

int
mas_output_level_c( const duf_config_output_t * cfgout )
{
  return cfgout ? cfgout->stream.level : 0;
}

FILE *
mas_output_file_c( const duf_config_output_t * cfgout )
{
  return cfgout && cfgout->stream.out ? cfgout->stream.out : stdout;
}

int
mas_output_force_color_c( const duf_config_output_t * cfgout )
{
  return cfgout ? cfgout->stream.v.flag.force_color : 0;
}

int
mas_output_nocolor_c( const duf_config_output_t * cfgout )
{
  return cfgout ? cfgout->stream.v.flag.nocolor : 0;
}

int
duf_output_progress_c( const duf_config_output_t * cfgout )
{
  return cfgout ? cfgout->stream.v.flag.progress : 0;
}

#if 0
int
duf_output_use_binformat_c( duf_config_output_t * cfgout )
{
  return cfgout ? cfgout->stream.v.flag.use_binformat : 0;
}
#endif
