#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_option_extended.h"
#include "duf_option.h"

/* ###################################################################### */
#include "duf_option_cmd.h"
/* ###################################################################### */



int
_duf_execute_cmd_long( const char *string, const duf_longval_extended_t * xtable, unsigned xtable_size, char vseparator )
{
  int r = DUF_ERROR_OPTION;
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
  if ( name )
  {
    /* for ( int ilong = 0; duf_config->longopts_table[ilong].name && ilong < lo_extended_count; ilong++ ) */
    for ( int ilong = 0; xtable[ilong].o.name && ilong < xtable_size; ilong++ )
    {
      DUF_TRACE( options, 4, "compare name:`%s` ? `%s` at %d", xtable[ilong].o.name, name, ilong );
      if ( 0 == strcmp( name, xtable[ilong].o.name ) )
      {
        DUF_TRACE( options, 0, "found name:`%s` at %d", xtable[ilong].o.name, ilong );
        /* const duf_longval_extended_t *extended; */

        /* extended = _duf_find_longval_extended( duf_config->longopts_table[ilong].val ); */
        /* extended = &xtable[ilong]; */
        /* extended = duf_longindex_extended( ilong ); */

/* 
 * duf_parse_option_long return 
 *   =0 for other option
 *   errorcode<0 for error
 * */
        /* r = duf_parse_option_long( ilong, arg ); */
        r = duf_parse_option_long_x( &xtable[ilong], arg );
        DUF_TEST_R( r );
        break;
      }
    }
    /* DUF_TEST_R( r ); */
  }
  mas_free( name );
  mas_free( arg );
  if ( r == DUF_ERROR_OPTION )
  {
    DUF_ERROR( "Invalid option -- '%s'", string );
  }
  else
    DUF_TEST_R( r );
  return r;
}

int
duf_execute_cmd_long( const char *string, char vseparator )
{
  return _duf_execute_cmd_long( string, lo_extended, lo_extended_count, vseparator );
}
