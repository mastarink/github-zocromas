#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"


#include "duf_xtended_table.h"
#include "duf_option_clarify.h"
#include "duf_option_find.h"
#include "duf_option_stage.h"

/* TODO file rename duf_option_string_clarify.[ch] */

/* ###################################################################### */
#include "duf_option_cmd.h"
/* ###################################################################### */


/* look xtable for cmd from string and exec if found */
/* string + extended_array => find + clarify */
/* duf_soption_xclarify_at_xarr */
static int
duf_soption_xclarify_at_xarr( const char *string, const duf_longval_extended_table_t * xtable, char vseparator, duf_option_stage_t istage, int all_matched,
                     duf_option_source_t source )
{
  DEBUG_STARTR( r );
  int clarified = 0;
  int cntfound = 0;
  const duf_longval_extended_t *xtended = xtable->table;

  /* DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_PARSED, DUF_ERROR_OPTION_NOT_FOUND ); */
  while ( xtended /* && DUF_CLEARED_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ) */  && DUF_NOERROR( r ) )
  {
    const duf_longval_extended_t *extended = NULL;
    char *arg = NULL;
    int no = 0;

    DUF_TRACE( findopt, 5, "to find %s at %s [%ld]", string, xtable->name, xtended - xtable->table );
    extended = duf_soption_xfind_at_xarr( string, xtended, vseparator, &arg, &no, &cntfound, &r );
    DUF_TRACE( options, 190, "string:%s; no:%d extended(+-):%d", string, no, extended ? 1 : 0 );

    if ( extended && DUF_NOERROR( r ) )
    {
      DUF_TRACE( findopt, 5, "@(%s:%d) at %s by %s clarified cmd for %s", mas_error_name_i( r ), clarified, xtable->name, string, extended->o.name );
      DORF( r, DUF_WRAPPED( duf_xoption_clarify ), extended, arg, istage, xtable, no, source );
      DUF_TRACE( findopt, 5, "@(%s:%d) at %s by %s full done for %s", mas_error_name_i( r ), clarified, xtable->name, string, extended->o.name );
      clarified += ( extended ? 1 : 0 );
      DUF_TRACE( options, 130, "@executed '%s'; stage:%s; source:%d", extended->o.name, duf_optstage_name( istage ), source );
    }
    else
    {
      /* DUF_SHOW_ERRORO( "extended is NULL after duf_soption_xfind_at_xarr for %s", string ); */
    }
    mas_free( arg );
    arg = NULL;
    xtended = extended;
    if ( !all_matched )
      break;
    if ( xtended ) /* continue if matched all needed */
      xtended++;
  }
  /* assert( ( clarified && DUF_NOERROR( r ) ) || ( !clarified && DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_FOUND ) ) ); */
  if ( clarified )
  {
    /* DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
    /* if ( DUF_NOERROR( r ) ) */
    /*   r = clarified;            */
  }
  if ( clarified )
    DUF_TRACE( options, 130, "@executed at least one from '%s'", string );
  if ( !clarified && DUF_NOERROR( r ) )
    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
  /* DEBUG_ENDR_UPPER( r, DUF_ERROR_OPTION_NOT_PARSED, DUF_ERROR_OPTION_NOT_FOUND ); */
  DEBUG_ENDR( r );
}

/* look all multix for cmd from string and exec if found */
/* duf_soption_xclarify_at_multix */
static int
duf_soption_xclarify_at_multix( const char *string, const duf_longval_extended_table_t ** multix, char vseparator, duf_option_stage_t istage,
                           int all_matched, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  int clarified = 0;
  const duf_longval_extended_table_t *xtable = NULL;

  DUF_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND );

  /* find and execute ALL matching commands */
  while ( ( xtable = *multix++ ) && DUF_CLEARED_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ) )
  {
/* 
 * duf_soption_xclarify_at_xarr >0 if clarified
 * duf_soption_xclarify_at_xarr == 0 if ???
 * duf_soption_xclarify_at_xarr < 0 if error
 * */
/* look xtable for cmd from string and exec if clarified */
    DOR( r, duf_soption_xclarify_at_xarr( string, xtable, vseparator, istage, all_matched, source ) );
    DUF_TRACE( options, 150, "@after executing cmd from '%s'; table %s. (%d:%s)" , string, xtable->name, r, mas_error_name_i( r ) );
    if ( DUF_NOERROR( r ) )     /* DUF_NOERROR(r) equal to r>=0 ?? */
    {
      /* clarified one; continue to possibly find more */
      clarified += 1;
      /* r = 0; */
    }
    /* if ( DUF_NOERROR( r ) ) */
    /*   break;      */
    DUF_TRACE( options, 170, "@@at %s (%d:%s)", xtable->name ? xtable->name : "??", r, mas_error_name_i( r ) );
  }
  /* no error if at least one clarified */
  if ( clarified )
    DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
  DUF_TRACE( options, 160, "@@@executed cmd from '%s' (%d:%s)", string, r, mas_error_name_i( r ) );
  if ( DUF_IS_ERROR_N( r, DUF_ERROR_OPTION ) || DUF_IS_ERROR_N( r, DUF_ERROR_OPTION_NOT_FOUND ) )
  {
    DUF_SHOW_ERRORO( "@@@@@@@Invalid command -- '%s' at %s stage", string, duf_optstage_name( istage ) );
    /* TODO DUF_POP_ERROR( r ); */
  }

  DUF_E_UPPER( DUF_ERROR_OPTION_NOT_FOUND );

  DEBUG_ENDR( r );
}

/* look all std xtables for cmd from string and exec if found */
int
duf_soption_xclarify_at_stdx( const char *string, char vseparator, duf_option_stage_t istage, int all_matched, duf_option_source_t source )
{
  DEBUG_STARTR( r );
  DEBUG_E_LOWER( DUF_ERROR_OPTION_NOT_FOUND, DUF_ERROR_MAX_SEQ_REACHED );
/* look all xtables for cmd from string and exec if found */
  DOR( r, duf_soption_xclarify_at_multix( string, duf_extended_table_multi(  ), vseparator, istage, all_matched, source ) );
  DUF_TRACE( options, 160, "@executed cmd '%s'; stage:%s; source:%d (%d:%s)", string, duf_optstage_name( istage ), source, r, mas_error_name_i( r ) );
  DEBUG_ENDR_UPPER( r, DUF_ERROR_OPTION_NOT_FOUND, DUF_ERROR_MAX_SEQ_REACHED );
}
