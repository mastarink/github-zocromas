#include <string.h>

#include "duf_maintenance_options.h"


#include "duf_option_clarify_string.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"

/* ###################################################################### */
#include "duf_options_string.h"
#include "duf_options_env.h"
/* ###################################################################### */



static int
duf_env_options_at_var( duf_option_stage_t istage, const char *envvarname )
{
  DEBUG_STARTR( r );

  const char *eo = NULL;

  eo = getenv( envvarname );

  DUF_TRACE( options, 1, "@@@@@@@stage:%s; source: env(%s='%s')", duf_optstage_name( istage ), envvarname, eo );
  DOR( r, duf_boption_xclarify_at_stdx( '=', istage, DUF_OPTION_SOURCE_ENV, eo, 0 ) );

  DUF_TRACE( explain, 0, "got env options from %s", envvarname );

  DEBUG_ENDR( r );
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
                        __attribute__ ( ( unused ) ), duf_cpchar_void_func_t cb_prompt_interactive __attribute__ ( ( unused ) ) )
{
  DEBUG_STARTR( r );

  DOR( r, duf_env_options_at_var( istage, "MSH_DUF_AOPTIONS" ) );
  DOR( r, duf_env_options_at_var( istage, "MSH_DUF_ZOPTIONS" ) );
  DOR( r, duf_env_options_at_var( istage, "MSH_DUF_OPTIONS" ) );
  DEBUG_ENDR( r );
}
