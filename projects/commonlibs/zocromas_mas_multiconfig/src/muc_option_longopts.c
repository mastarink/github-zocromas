/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
/* #include "muc_tracen_defs_preset.h" */

#include <assert.h>                                                  /* assert */
#include <string.h>                                                  /* memset */

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/trace/mas_trace.h> */
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include "muc_option_extended.h"                                     /* muc_longindex_extended_count etc. ♠ */

/* #include "muc_config_cli_structs.h" */
/* ###################################################################### */
#include "muc_option_longopts.h"                                     /* muc_options_create_longopts_table ♠ */
/* ###################################################################### */

void
muc_options_delete_longopts_table( muc_option_t * longopts )
{
  for ( muc_option_t * o = longopts; o->name; o++ )
    mas_free( ( char * ) o->name );
  mas_free( longopts );
}

static void
muc_xtable2options( muc_option_t ** plongopts_ptr, const muc_longval_extended_t * xtended, unsigned noo )
{
  while ( xtended->o.name )
  {
    assert( xtended->o.val );                                        /* TODO => MAKE_ERROR */
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
 * array of muc_longval_extended_vtable_t => allocated array of muc_option_t
 * objects: 
 *     muc_longval_extended_vtable_t
 *     muc_option_t
 *
 *     TODO: file ..._option_longopts_credel.c
 * */
muc_option_t *
muc_options_create_longopts_table( muc_longval_extended_vtable_t ** xvtables )
{
  muc_option_t *longopts = NULL;

  {
    muc_option_t *longopts_ptr;
    size_t tbsize = 0;

    tbsize = muc_longindex_extended_count( xvtables ) * ( sizeof( muc_longval_extended_t ) + 1 );

    longopts = longopts_ptr = mas_malloc( tbsize );
    memset( longopts, 0, tbsize );

    for ( muc_longval_extended_vtable_t ** xvtable = xvtables; *xvtable; xvtable++ )
      muc_xtable2options( &longopts_ptr, ( *xvtable )->xlist, 0 /* noo */  );
    for ( muc_longval_extended_vtable_t ** xvtable = xvtables; *xvtable; xvtable++ )
      muc_xtable2options( &longopts_ptr, ( *xvtable )->xlist, 1 /* noo */  );
  }
  return longopts;
}
