/* #include <stdarg.h> */
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_utils_path.h>

#include "duf_maintenance.h"

#include "duf_config_wref.h"

#include "duf_dbg.h"
#include "duf_pdi_credel.h"

#include "duf_ufilter.h"

/* #include "duf_option_extended.h" */
#include "duf_option_names.h"

/* #include "duf_config_ref.h" */
/* ###################################################################### */
#include "duf_config_util.h"
/* ###################################################################### */

extern duf_config_t *duf_config /* __attribute__( ( weak ) ) */ ;

duf_config_t *
duf_get_config( void )
{
  assert( duf_config );
  return duf_config;
}

FILE *
duf_output_file_c( const duf_config_t * cfg )
{
  return cfg && cfg->opt.output.out ? cfg->opt.output.out : stdout;
}

FILE *
duf_trace_file_c( const duf_config_t * cfg )
{
  return cfg && cfg->MAST_TRACE_AT_CFG.output.out ? cfg->MAST_TRACE_AT_CFG.output.out : stdout;
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
