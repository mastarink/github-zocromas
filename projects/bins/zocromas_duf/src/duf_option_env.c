#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_option_extended.h"
#include "duf_option_cmd.h"

/* ###################################################################### */
#include "duf_options.h"
/* ###################################################################### */



int
duf_env_options_at_var( duf_option_stage_t istage, const char *envvarname )
{
  DEBUG_STARTR( r );
  const char *eo = NULL;
  const char *peo, *e;

  eo = getenv( envvarname );

  DUF_TRACE( options, 0, "getting env options from variable %s", envvarname );
  peo = eo;
  while ( peo && *peo )
  {
    char *s;

    s = NULL;
    e = strchr( peo, ':' );
    if ( e )
    {
      s = mas_strndup( peo, e - peo );
      DUF_TRACE( explain, 0, "env option from \"%s\"", s );
      e++;
    }
    else
    {
      s = mas_strdup( peo );
      DUF_TRACE( explain, 0, "env option (last) from \"%s\"", s );
    }
    if ( s )
    {
      char *xs;

      DUF_TRACE( explain, 0, "env s: \"%s\"", s );
      xs = mas_expand_string( s );
      DUF_TRACE( explain, 0, "env xs: \"%s\"", xs );
      DOR( r, duf_execute_cmd_long_xtables_std( xs, '=', istage ) );
      mas_free( xs );
    }
    mas_free( s );
    peo = e;
    /* DUF_TRACE( explain, 0, "env peo \"%s\"", peo ); */
    DUF_TRACE( explain, 0, "got env options from %s", envvarname );
  }
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
  const char *varname = "MSH_DUF_OPTIONS";

  return duf_env_options_at_var( istage, varname );
}
