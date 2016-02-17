#include <string.h>

#include "duf_maintenance_options.h"

#include "duf_config.h"
#include "duf_config_util.h"
#include "duf_config_trace.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"

/* #include "duf_option_clarify_string.h" */
#include "duf_option_clarify_batch.h"

/* ###################################################################### */
#include "duf_options_env.h"
/* ###################################################################### */



static int
duf_env_options_at_var( duf_option_stage_t istage, const char *envvarname, duf_option_adata_t * paod )
{
  DUF_STARTR( r );

  const char *eo = NULL;

  eo = getenv( envvarname );

  DUF_TRACE( options, 1, "@@@@@@@stage:%s; source: env(%s='%s')", duf_optstage_name( istage ), envvarname, eo );
  DOR( r, duf_boption_xclarify_at_stdx( '=', istage, DUF_OPTION_SOURCE( ENV ), eo, 0, paod ) );

  DUF_TRACE( explain, 0, "got env options from %s", envvarname );

  DUF_ENDR( r );
}

/*
 * get options/settings from OS environment
 *    - global variable duf_config must be created/inited
 * ****************************************
 * 1. set OS environment variable name
 * 2. call duf_env_options_at_var
 * */
int
duf_source_env_parse( duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
                      __attribute__ ( ( unused ) ), duf_cpchar_void_func_t cb_prompt_interactive DUF_UNUSED, duf_option_adata_t * paod,
                      duf_option_source_code_t sourcecode  DUF_UNUSED)
{
  /* extern duf_config_t *duf_config; */

  DUF_STARTR( r );
  DUF_TRACE( optsource, 0, "@   source:%s", duf_optsourcecode_name( sourcecode ) );
  DOR( r, duf_env_options_at_var( istage, "MSH_DUF_AOPTIONS", paod ) );
  DOR( r, duf_env_options_at_var( istage, "MSH_DUF_ZOPTIONS", paod ) );
  DOR( r, duf_env_options_at_var( istage, "MSH_DUF_OPTIONS", paod ) );
  DUF_ENDR( r );
}
