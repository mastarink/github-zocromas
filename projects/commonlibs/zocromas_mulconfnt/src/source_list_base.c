#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_structs.h"

#include "source_base.h"
#include "source_list_base.h"

/*
 *
 * mucs_source_list_...
 *
 * */

void
mucs_source_list_init( mucs_source_list_t * source_list )
{
  memset( source_list, 0, sizeof( mucs_source_list_t ) );
}

mucs_source_list_t *
mucs_source_list_create( void )
{
  mucs_source_list_t *source_list = mas_calloc( 1,sizeof( mucs_source_list_t ) );

  mucs_source_list_init( source_list );
  return source_list;
}

void
mucs_source_list_reset( mucs_source_list_t * source_list )
{
  if ( source_list )
  {
    mucs_source_t *osrc = source_list->first;

    while ( osrc )
    {
      mucs_source_t *next = osrc->next;

      mucs_source_delete( osrc );
      osrc = next;
    }
    source_list->first = NULL;
    mas_argvc_delete( &source_list->targ );
  }
}

void
mucs_source_list_delete( mucs_source_list_t * source_list )
{
  mucs_source_list_reset( source_list );
  if ( source_list )
    mas_free( source_list );
}
