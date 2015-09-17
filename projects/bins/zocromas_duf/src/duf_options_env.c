#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


/* #include "duf_option_extended.h" */
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
#if 0
  {
    const char *peo, *e;

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

        DOR( r, duf_exec_cmd_long_xtables_std( xs, '=', istage, DUF_OPTION_SOURCE_ENV ) );

        mas_free( xs );
      }
      mas_free( s );
      peo = e;
      /* DUF_TRACE( explain, 0, "env peo \"%s\"", peo ); */
    }
  }
#else
  DOR( r, duf_string_options_at_string( '=', istage, DUF_OPTION_SOURCE_ENV, eo, 0 ) );
#endif
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
