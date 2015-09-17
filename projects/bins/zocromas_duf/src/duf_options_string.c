#include <string.h>
#include <time.h>


#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


/* #include "duf_option_extended.h" */
#include "duf_option_cmd.h"

/* ###################################################################### */
#include "duf_options_string.h"
/* ###################################################################### */

static const char *
duf_string_options_at_string_xsdb_getvar( const char *name, const char *arg )
{
  static char buf[256];
  size_t len;
  size_t llen;
  const char *label = "TIME(";

  llen = strlen( label );
  *buf = 0;
  len = strlen( name );
  if ( len > llen && 0 == strncmp( name, "TIME(", llen ) && name[len - 1] == ')' )
  {
    /* strftime */
    char *fmt;

    fmt = mas_strndup( name + llen, len - llen - 1 );
    mas_tstrflocaltime( buf, sizeof( buf ), fmt, time( NULL ) );
    mas_free( fmt );
  }
  return buf;
}

/*
 * if DUF_CONFIGG(cli.option_delimiter) NOT set
 * "::trace-path=1:trace-options=1"  -- correct
 * ":;trace-path=1;trace-options=1"  -- correct
 * ":,trace-path=1,trace-options=1"  -- correct
 * "trace-path=1:trace-options=1"    -- correct
 *
 * */
int
duf_string_options_at_string( char vseparator, duf_option_stage_t istage, duf_option_source_t source, const char *cmdstr, char delim )
{
  DEBUG_STARTR( r );

  if ( !delim )
    delim = DUF_CONFIGG( cli.option_delimiter );
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
        xs = mas_expand_string_cb_arg( s, duf_string_options_at_string_xsdb_getvar, NULL );
        {
          char *xs1;

          xs1 = mas_expand_string( xs );
          mas_free( xs );
          xs = xs1;
        }
        DUF_TRACE( explain, 0, "xs: \"%s\"", xs );

        DOR( r, duf_exec_cmd_long_xtables_std( xs, vseparator, istage, source ) );

        mas_free( xs );
      }
      mas_free( s );
      peo = e;
      /* DUF_TRACE( explain, 0, "peo \"%s\"", peo ); */
    }
  }
  DEBUG_ENDR( r );
}
