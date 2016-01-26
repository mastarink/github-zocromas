#include <string.h>

#include "duf_maintenance_options.h"

#include "duf_option_clarify_string.h"

#include "duf_option_clarify_new.h"
#include "duf_option_source.h"

/* ###################################################################### */
#include "duf_option_clarify_batch.h"
/* ###################################################################### */


/*
 * if DUF_CONFIGG(opt.option_delimiter) NOT set
 * "::trace-path=1:trace-options=1"  -- correct
 * ":;trace-path=1;trace-options=1"  -- correct
 * ":,trace-path=1,trace-options=1"  -- correct
 * "trace-path=1:trace-options=1"    -- correct
 *
 * */
/* look all std xtables for cmd's separated with vseparator from string and exec if found */
/* duf_boption_xclarify_at_stdx */
int
duf_boption_xclarify_at_stdx( char vseparator, duf_option_stage_t istage, duf_option_source_t source, const char *cmdstr, char delim )
{
  DEBUG_STARTR( r );

  if ( !delim )
    /* delim = DUF_CONFIGG( opt.option_delimiter ); */
    delim = duf_option_delimiter(  );
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

#if 0
        DUF_TRACE( explain, 0, "s: \"%s\"", s );
        xs = mas_expand_string_cb_arg( s, duf_string_options_at_string_xsdb_getvar, NULL );
        {
          char *xs1;

          xs1 = mas_expand_string( xs );
          mas_free( xs );
          xs = xs1;
        }
#else
        /* xs = _duf_string_options_expand( s, &expandable_later ); */
        /* xs = duf_string_options_expand( s, '?' ); */
#endif
        DUF_TRACE( explain, 0, "s: \"%s\"", s );

/* look all std xtables for cmd from string and exec if found */


/* TODO */
#if 0                           /* 1:old clarify; 0:new clarify; see also duf_options_cli.c ....; 20160115.170514 */
        DOR( r, duf_soption_xclarify_at_stdx( s, vseparator, istage, 0 /* all_matched */ , source ) );
#else
        /* T( "%s; source:%s", s, duf_optsource_name(source) ); */
        DORF( r, F2N( soption_xclarify_new_at_stdx_default ), s, NULL, NULL, vseparator, istage, source );
#endif
      }
      mas_free( s );
      peo = e;
      /* DUF_TRACE( explain, 0, "peo \"%s\"", peo ); */
    }
  }
  DEBUG_ENDR( r );
}
