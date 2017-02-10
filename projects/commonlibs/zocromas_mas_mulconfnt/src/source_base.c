#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "source_defaults.h"
#include "source_base.h"

void
mulconfnt_source_init( config_source_desc_t * osrc )
{
  memset( osrc, 0, sizeof( config_source_desc_t ) );
}

config_source_desc_t *
mulconfnt_source_create( void )
{
  config_source_desc_t *osrc = mas_malloc( sizeof( config_source_desc_t ) );

  mulconfnt_source_init( osrc );
  return osrc;
}

config_source_desc_t *
mulconfnt_source_create_set( config_source_t source_type, int count, void *data_ptr, const char *delims,
                             const char *eq, const config_prefix_encoder_t * pref_ids )
{
  config_source_desc_t *osrc = NULL;

  if ( source_type >= 0 && source_type < mulconfnt_source_defaults_count(  ) )
  {
/* const config_source_desc_t *defsrc = &default_sources[source_type]; */
    const config_source_desc_t *defsrc = mulconfnt_source_default( source_type );

    if ( defsrc && source_type == defsrc->type )
    {
      int check = !defsrc->check_fun || defsrc->check_fun( count, data_ptr, delims, eq, pref_ids );

      if ( check )
      {
        osrc = mulconfnt_source_create(  );

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
    }
  }
  return osrc;
}

void
mulconfnt_source_close( config_source_desc_t * osrc )
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
    mas_argvc_delete( &osrc->targno );
  }
}

void
mulconfnt_source_delete( config_source_desc_t * osrc )
{
  if ( osrc )
  {
    mulconfnt_source_close( osrc );
    mas_free( osrc );
  }
}
