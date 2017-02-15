#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "source_base.h"
#include "source_list_base.h"

void
mucs_source_list_init( config_source_list_t * source_list )
{
  memset( source_list, 0, sizeof( config_source_list_t ) );
}

config_source_list_t *
mucs_source_list_create( void )
{
  config_source_list_t *source_list = mas_malloc( sizeof( config_source_list_t ) );

  mucs_source_list_init( source_list );
  return source_list;
}

void
mucs_source_list_close( config_source_list_t * source_list )
{
  if ( source_list )
  {
    config_source_desc_t *osrc = source_list->first;

    while ( osrc )
    {
      config_source_desc_t *next = osrc->next;

      mucs_source_delete( osrc );
      osrc = next;
    }
    source_list->first = NULL;
    mas_argvc_delete( &source_list->targ );
  }
}

void
mucs_source_list_delete( config_source_list_t * source_list )
{
  mucs_source_list_close( source_list );
  if ( source_list )
    mas_free( source_list );
}
