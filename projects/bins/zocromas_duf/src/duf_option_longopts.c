#include <string.h>
#include <stddef.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_xtended_table.h"
#include "duf_option_stage.h"   /* duf_optstage_name */

#include "duf_option_extended.h"

/* ###################################################################### */
#include "duf_option_longopts.h"
/* ###################################################################### */

void
duf_options_delete_longopts_table( duf_option_t * longopts )
{
  for ( duf_option_t * o = longopts; o->name; o++ )
    mas_free( ( char * ) o->name );
  mas_free( longopts );
}

static void
duf_xtable2options( duf_option_t ** plongopts_ptr, const duf_longval_extended_t * xtended, unsigned noo )
{
  while ( xtended->o.name )
  {
    assert( xtended->o.val );   /* TODO => MAKE_ERROR */
    if ( noo )
    {
      if ( xtended->can_no )
      {
        char *s = NULL;

        *( *plongopts_ptr ) = xtended->o;
        s = mas_strdup( "no-" );
        s = mas_strcat_x( s, ( *plongopts_ptr )->name );
        ( *plongopts_ptr )->name = s;
        /* ( *plongopts_ptr )->has_arg = xtended->o.has_arg; */
        /* ( *plongopts_ptr )->val = xtended->o.val;         */
        ( *plongopts_ptr )++;
      }
    }
    else
    {
      *( *plongopts_ptr ) = xtended->o;
      ( *plongopts_ptr )->name = mas_strdup( ( *plongopts_ptr )->name );
      /* ( *plongopts_ptr )->has_arg = xtended->o.has_arg; */
      /* ( *plongopts_ptr )->val = xtended->o.val;         */
      ( *plongopts_ptr )++;
    }
    xtended++;
  }
}

/* 
 * array of duf_longval_extended_table_t => allocated array of duf_option_t
 * objects: 
 *     duf_longval_extended_table_t
 *     duf_option_t
 * */
duf_option_t *
duf_options_create_longopts_table( const duf_longval_extended_table_t ** pxtable )
{
  duf_option_t *longopts = NULL;

  {
    duf_option_t *longopts_ptr;
    size_t tbsize = 0;

    tbsize = duf_longindex_extended_count( pxtable ) * ( sizeof( duf_longval_extended_t ) + 1 );

    longopts = longopts_ptr = mas_malloc( tbsize );
    memset( longopts, 0, tbsize );

    for ( const duf_longval_extended_table_t ** xtables = pxtable; *xtables; xtables++ )
      duf_xtable2options( &longopts_ptr, ( *xtables )->table, 0 );
    for ( const duf_longval_extended_table_t ** xtables = pxtable; *xtables; xtables++ )
      duf_xtable2options( &longopts_ptr, ( *xtables )->table, 1 );
  }
  return longopts;
}
