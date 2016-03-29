#include <string.h>
#include <stdio.h>                                                   /* FILE */
#include <errno.h>

/* #include <mastar/tools/mas_tools.h>                                  (* mas_tstrflocaltime *) */
/* #include <mastar/tools/mas_utils_path.h>                             (* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ *) */


#include "duf_config.h"                                              /* duf_get_config ✗ */

#include "duf_cfg_output_util.h"
/* ###################################################################### */
#include "duf_config_output_util.h"
/* ###################################################################### */

int
mas_output_level( void )
{
  return mas_output_level_c( duf_get_config(  ) );
}

FILE *
mas_output_file( void )
{
  return mas_output_file_c( duf_get_config(  ) );
}

int
mas_output_force_color( void )
{
  return mas_output_force_color_c( duf_get_config(  ) );
}

int
mas_output_nocolor( void )
{
  return mas_output_nocolor_c( duf_get_config(  ) );
}

int
duf_output_progress( void )
{
  return duf_output_progress_c( duf_get_config(  ) );
}

#if 0
int
duf_output_use_binformat( void )
{
  return duf_output_use_binformat_c( duf_get_config(  ) );
}
#endif
