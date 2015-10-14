#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"



#include "duf_option_cmd.h"

/* ###################################################################### */
#include "duf_options_string.h"
#include "duf_options_env.h"
/* ###################################################################### */



int
duf_env_options_at_var( duf_option_stage_t istage, const char *envvarname )
{
  DEBUG_STARTR( r );
  
  const char *eo = NULL;

  eo = getenv( envvarname );

  DUF_TRACE( options, 0, "@@@@(%d) source: env(%s='%s')", istage, envvarname, eo );
  DOR( r, duf_string_options_at_string( '=', istage, DUF_OPTION_SOURCE_ENV, eo, 0 ) );
  
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
duf_env_options( duf_option_stage_t istage )
{
  DEBUG_STARTR( r );

  DOR( r, duf_env_options_at_var( istage, "MSH_DUF_AOPTIONS" ) );
  DOR( r, duf_env_options_at_var( istage, "MSH_DUF_ZOPTIONS" ) );
  DOR( r, duf_env_options_at_var( istage, "MSH_DUF_OPTIONS" ) );
  DEBUG_ENDR( r );
}
