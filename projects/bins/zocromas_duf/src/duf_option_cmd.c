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
duf_cli_getcmd_long( const char *string )
{
  int r = DUF_ERROR_OPTION;
  char *eq;
  char *name = NULL;
  char *arg = NULL;

  eq = strchr( string, '=' );
  if ( eq )
  {
    name = mas_strndup( string, eq - string );
    arg = mas_strdup( eq + 1 );
  }
  else
  {
    name = mas_strdup( string );
  }
  DUF_TRACE( explain, 0, "name:`%s`; arg:`%s`", name, arg );
  if ( name )
  {
    /* for ( int ilong = 0; duf_config->longopts_table[ilong].name && ilong < lo_extended_count; ilong++ ) */
    for ( int ilong = 0; lo_extended[ilong].o.name && ilong < lo_extended_count; ilong++ )
    {
      if ( 0 == strcmp( name, lo_extended[ilong].o.name ) )
      {
        DUF_TRACE( explain, 0, "found name:`%s` at %d", lo_extended[ilong].o.name, ilong );
        /* const duf_longval_extended_t *extended; */

        /* extended = _duf_find_longval_extended( duf_config->longopts_table[ilong].val ); */
        /* extended = &lo_extended[ilong]; */
        /* extended = duf_longindex_extended( ilong ); */

/* 
 * duf_parse_option_long return 
 *   =0 for other option
 *   errorcode<0 for error
 * */
        /* r = duf_parse_option_long( ilong, arg ); */
        r = duf_parse_option_long_x( &lo_extended[ilong], arg );
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
