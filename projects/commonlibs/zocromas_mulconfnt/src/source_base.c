#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

/* #include "mulconfnt_error_base.h" */
/* #include "mulconfnt_error.h" */

#include "source_defaults.h"
#include "source_base.h"

/*
 *
 * mucs_source_...
 *
 * */

void
mucs_source_init( mucs_source_t * osrc )
{
  memset( osrc, 0, sizeof( mucs_source_t ) );
}

mucs_source_t *
mucs_source_create( void )
{
  mucs_source_t *osrc = mas_calloc( 1, sizeof( mucs_source_t ) );

  mucs_source_init( osrc );
  return osrc;
}

mucs_source_t *
mucs_source_create_setup( mucs_source_type_t source_type, int count, const void *data_ptr, const char *delims,
                          const char *eq, const mucs_prefix_encoder_t * pref_ids )
{
  mucs_source_t *osrc = NULL;

  if ( source_type >= 0 && source_type < mucs_source_defaults_count(  ) )
  {
/* const mucs_source_t *defsrc = &default_sources[source_type]; */
    const mucs_source_t *defsrc = mucs_source_default( source_type );

    if ( defsrc && source_type == defsrc->type )
    {
      int check = !defsrc->check_fun || defsrc->check_fun( count, data_ptr, delims, eq, pref_ids );

      if ( check )
      {
        osrc = mucs_source_create(  );

        osrc->type = source_type;
        osrc->count = count;

        osrc->delim = defsrc->delim;

        if ( delims )
          osrc->delims = mas_strdup( delims );
        else if ( defsrc->delims )
          osrc->delims = mas_strdup( defsrc->delims );

        if ( eq )
          osrc->eq = mas_strdup( eq );
        else if ( defsrc->eq )
          osrc->eq = mas_strdup( defsrc->eq );

        for ( unsigned i = 0; i < sizeof( osrc->pref_ids ) / sizeof( osrc->pref_ids[0] ); i++ )
        {
          osrc->pref_ids[i] = pref_ids ? pref_ids[i] : defsrc->pref_ids[i];
          if ( osrc->pref_ids[i].string )
            osrc->pref_ids[i].string = mas_strdup( osrc->pref_ids[i].string );
        }
        osrc->data_ptr = data_ptr ? data_ptr : defsrc->data_ptr;
        osrc->check_fun = defsrc->check_fun;
        osrc->open_fun = defsrc->open_fun;
        osrc->close_fun = defsrc->close_fun;
        osrc->load_string_fun = defsrc->load_string_fun;
        osrc->load_targ_fun = defsrc->load_targ_fun;
        osrc->next = NULL;
      }
/*    else
        DIE( "FATAL ERROR: can't create \"source\" check:%d [%p] %d\n", check, defsrc ? defsrc->check_fun : NULL, defsrc ? defsrc->type : 0xffff ); */
    }
    else
      DIE( "FATAL ERROR: can't create \"source\" %p; %d ? %d\n", defsrc, source_type, defsrc ? defsrc->type : 0xffff );
  }
  else
    DIE( "FATAL ERROR: can't create \"source\" %d ? %lu\n", source_type, mucs_source_defaults_count(  ) );
  return osrc;
}

void
mucs_source_reset( mucs_source_t * osrc )
{
  if ( osrc )
  {
    if ( osrc->delims )
      mas_free( osrc->delims );
    if ( osrc->eq )
      mas_free( osrc->eq );
    for ( unsigned i = 0; i < sizeof( osrc->pref_ids ) / sizeof( osrc->pref_ids[0] ); i++ )
    {
      if ( osrc->pref_ids[i].string )
        mas_free( osrc->pref_ids[i].string );
      osrc->pref_ids[i].string = NULL;
    }
    if ( osrc->string )
      mas_free( osrc->string );
    osrc->string = NULL;
    mas_argvc_delete( &osrc->targ );
    mas_argvc_delete( &osrc->oldtarg );
    mas_argvc_delete( &osrc->targno );
  /* mucs_error_reset( &osrc->error ); */
  }
}

void
mucs_source_delete( mucs_source_t * osrc )
{
  mucs_source_reset( osrc );
  if ( osrc )
    mas_free( osrc );
}
