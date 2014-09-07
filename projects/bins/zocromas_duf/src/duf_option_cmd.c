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
_duf_cli_getcmd_long( const char *string, const duf_longval_extended_t * xtable, unsigned xtable_size )
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
duf_cli_getcmd_long( const char *string )
{
  return _duf_cli_getcmd_long( string, lo_extended, lo_extended_count );
}
