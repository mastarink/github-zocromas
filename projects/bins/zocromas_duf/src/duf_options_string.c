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
#include "duf_options_string.h"
/* ###################################################################### */


/*
 * if duf_config->cli.option_delimiter NOT set
 * "::trace-path=1:trace-options=1"  -- correct
 * ":;trace-path=1;trace-options=1"  -- correct
 * ":,trace-path=1,trace-options=1"  -- correct
 * "trace-path=1:trace-options=1"    -- correct
 *
 * */
int
duf_string_options_at_string( duf_option_stage_t istage, duf_option_source_t source, const char *cmdstr, char delim )
{
  DEBUG_STARTR( r );

  if ( !delim )
    delim = duf_config->cli.option_delimiter;
  if ( !delim )
    delim = ':';
  if ( cmdstr && *cmdstr == ':' )
  {
    cmdstr++;
    delim = *cmdstr++;
  }
  {
    const char *peo, *e;

    peo = cmdstr;
    while ( peo && *peo )
    {
      char *s;

      s = NULL;
      e = strchr( peo, delim );
      if ( e )
      {
        s = mas_strndup( peo, e - peo );
        DUF_TRACE( explain, 0, "option from \"%s\"", s );
        e++;
      }
      else
      {
        s = mas_strdup( peo );
        DUF_TRACE( explain, 0, "option (last) from \"%s\"", s );
      }
      if ( s )
      {
        char *xs;

        DUF_TRACE( explain, 0, "s: \"%s\"", s );
        xs = mas_expand_string( s );
        DUF_TRACE( explain, 0, "xs: \"%s\"", xs );

        DOR( r, duf_exec_cmd_long_xtables_std( xs, '=', istage, source ) );

        mas_free( xs );
      }
      mas_free( s );
      peo = e;
      /* DUF_TRACE( explain, 0, "peo \"%s\"", peo ); */
    }
  }
  DEBUG_ENDR( r );
}
