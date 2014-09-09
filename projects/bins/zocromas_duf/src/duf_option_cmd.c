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



int
duf_find_cmd_long( const char *string, const duf_longval_extended_t * xtable, unsigned xtable_size, char vseparator, char **parg )
{
  DEBUG_STARTR( r );
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

  DUF_TRACE( options, 0, "vseparator:'%c'; name:`%s`; arg:`%s`", vseparator, name, arg );

  DORN( r, duf_find_name_long( name, xtable, xtable_size, 1 /* soft */ ) );
  if ( r >= 0 && parg )
    *parg = arg;
  else
    mas_free( arg );
  arg = NULL;

  if ( r == DUF_ERROR_OPTION )
  {
    DUF_ERROR( "Invalid option -- '%s'", string );
  }
  mas_free( name );
  DEBUG_ENDRN( r );
}

int
duf_execute_cmd_long( const char *string, const duf_longval_extended_t * xtable, unsigned xtable_size, char vseparator, int stage )
{
  DEBUG_STARTR( r );
  const duf_longval_extended_t *extended = NULL;
  char *arg = NULL;

  r = duf_find_cmd_long( string, xtable, xtable_size, vseparator, &arg );
  if ( r >= 0 )
  {
    extended = &xtable[r];
    r = 0;
  }
/* 
 * duf_parse_option_long return 
 *   =0 for other option
 *   errorcode<0 for error
 * */
  DOR( r, duf_parse_option_long_typed( extended, arg, stage ) );
  if ( r == DUF_ERROR_OPTION_NOT_PARSED )
    DOZR( r, duf_parse_option_long_old( extended, arg, stage ) );
  mas_free( arg );
  DEBUG_ENDR( r );
}

int
duf_execute_cmd_long_std( const char *string, char vseparator, int stage )
{
  return duf_execute_cmd_long( string, lo_extended, lo_extended_count, vseparator, stage );
}
