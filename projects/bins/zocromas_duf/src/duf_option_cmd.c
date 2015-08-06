#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_option_find.h"
#include "duf_option_extended.h"
#include "duf_option.h"

/* ###################################################################### */
#include "duf_option_cmd.h"
/* ###################################################################### */


static const duf_longval_extended_t *
duf_find_cmd_long_no( const char *string, const duf_longval_extended_t * xtended, char vseparator, char **parg, int *pno, int *pr )
{
  const duf_longval_extended_t *extended = NULL;
  int r = 0;
  char *barg = NULL;
  char *endn = NULL;
  char *name = NULL;
  char *arg = NULL;

  switch ( vseparator )
  {
  case ' ':
    barg = endn = strpbrk( string, "\t\r\n " );
    while ( barg && *barg && strchr( "\t\r\n ", *barg ) )
      barg++;
    break;
  default:
    barg = endn = strchr( string, '=' );
    if ( barg )
      barg++;
    break;
  }
  if ( endn )
    name = mas_strndup( string, endn - string );
  else
    name = mas_strdup( string );
  if ( barg )
    arg = mas_strdup( barg );

  DUF_TRACE( options, 6, "vseparator:'%c'; name:`%s`; arg:`%s`", vseparator, name, arg );

  extended = duf_find_name_long_no( name, arg ? 1 : 0, xtended, 1 /* soft */ , pno, &r );
  if ( r >= 0 && parg )
    *parg = arg;
  else
    mas_free( arg );
  arg = NULL;

  mas_free( name );
  if ( pr )
    *pr = r;
  if ( extended )
    DUF_TRACE( options, 2, "@@(%s) found name:`%s`", duf_error_name( r ), extended->o.name );
  return extended;
}

int
duf_execute_cmd_long_xtable( const char *string, const duf_longval_extended_table_t * xtable, char vseparator, duf_option_stage_t istage )
{
  DEBUG_STARTR( r );
  const duf_longval_extended_t *extended = NULL;
  int found = 0;
  const duf_longval_extended_t *xtended = xtable->table;

  DEBUG_E_NO( DUF_ERROR_OPTION_NOT_PARSED, DUF_ERROR_OPTION_NOT_FOUND );
  do
  {
    char *arg = NULL;
    int no = 0;

    extended = duf_find_cmd_long_no( string, xtended, vseparator, &arg, &no, &r );
    DUF_TRACE( options, 1, "string:%s; no:%d extended(+-):%d", string, no, extended ? 1 : 0 );

    if ( extended )
    {
      DUF_TRACE( options, 3, "@(%s) found cmd #%d for %s", duf_error_name( r ), found, extended->o.name );
      DORF( r, DUF_WRAPPED( duf_interpret_option_long_full ), extended, arg, istage, xtable, no );
      DUF_TRACE( options, 1, "@(%s) full #%d done for %s", duf_error_name( r ), found, extended->o.name );
      found += ( extended ? 1 : 0 );
    }
    else
    {
      /* DUF_SHOW_ERROR( "extended is NULL after duf_find_cmd_long_no for %s", string ); */
    }
    mas_free( arg );
    arg = NULL;
    xtended = extended;
    if ( xtended )
      xtended++;
  }
  while ( xtended );
  if ( found )
    r = found;
  if ( found )
    DUF_TRACE( options, 3, "@executed" );
  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_PARSED, DUF_ERROR_OPTION_NOT_FOUND );
}

int
duf_execute_cmd_long_xtables( const char *string, const duf_longval_extended_table_t ** xtables, char vseparator, duf_option_stage_t istage )
{
  DEBUG_STARTR( r );
  int found = 0;
  const duf_longval_extended_table_t *xtable = NULL;

  while ( ( xtable = *xtables++ ) )
  {
    DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
    /* PF0( "table %s; search %s", xtable->o.name, string ); */
    DORN( r, duf_execute_cmd_long_xtable( string, xtable, vseparator, istage ) );
    DUF_TRACE( options, ( r >= 0 ) ? 1 : 5, "executed cmd; r=%d; xs=%s", r, string );
    if ( r > 0 )
      found += r;
    /* if ( r >= 0 ) */
    /*   break;      */
  }
  if ( found )
    DUF_CLEAR_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND );
  if ( r == DUF_ERROR_OPTION || r == DUF_ERROR_OPTION_NOT_FOUND )
  {
    DUF_SHOW_ERROR( "Invalid command -- '%s'", string );
  }
  DEBUG_ENDR( r );
}

int
duf_execute_cmd_long_xtables_std( const char *string, char vseparator, duf_option_stage_t istage )
{
  DEBUG_STARTR( r );
  DEBUG_E_NO( DUF_ERROR_OPTION_NOT_FOUND, DUF_ERROR_MAX_SEQ_REACHED );
  DOR( r, duf_execute_cmd_long_xtables( string, lo_extended_table_multi, vseparator, istage ) );
  DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND, DUF_ERROR_MAX_SEQ_REACHED );
}
