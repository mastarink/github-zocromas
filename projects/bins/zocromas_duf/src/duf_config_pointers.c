#include <string.h>
/* #include <errno.h> */

/* #include <mastar/tools/mas_tools.h>                                  (* mas_tstrflocaltime *) */
/* #include <mastar/tools/mas_utils_path.h>                             (* mas_concat_path *) */

/* #include "duf_expandable.h" */

#include "duf_config.h"
#include "duf_config_opt_structs.h"

/* #include "duf_config_defs.h" */
/* #include "duf_config_ref.h" */

#include "duf_config_util.h"
/* ###################################################################### */
#include "duf_config_pointers.h"
/* ###################################################################### */

void *
duf_get_duf_config_t_pointer( void )
{
  return duf_get_config(  );
}

void *
duf_get_duf_ufilter_t_pointer( void )
{
  return duf_get_config_ufilter(  );
}

void *
duf_get_muc_config_cli_t_pointer( void )
{
  return duf_get_config_cli(  );
}

void *
duf_get_mas_config_trace_t_pointer( void )
{
  return duf_get_config_opt(  )->ptracecfg;
}

void *
duf_get_duf_config_trace_enum_t_pointer( void )
{
  return duf_get_config_opt(  )->ptracecfg->class_levels;
}
