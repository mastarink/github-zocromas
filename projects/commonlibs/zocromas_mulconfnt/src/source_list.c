#include "mulconfnt_defs.h"
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"

#include "source_base.h"
#include "source.h"
#include "source_list.h"

/*
 *
 * mucs_source_list_...
 *
 * */

void
mucs_source_list_integrate( mucs_source_list_t * source_list )
{
  for ( mucs_source_han_t * osrc = source_list->first; osrc; osrc = osrc->next )
  {
    mucs_source_load_targ( osrc );
    mas_add_argvc_argvc( &source_list->targ, &osrc->targ, 0 );
  }
}

static void
mucs_source_list_add( mucs_source_list_t * source_list, mucs_source_han_t * osrc )
{
  if ( source_list && osrc )
  {
    mucs_source_han_t *os = source_list->first;

    if ( os )
    {
      while ( os && os->next )
        os = os->next;
      os->next = osrc;
    }
    else
      source_list->first = osrc;
    osrc->list = source_list;
  }
}

mucs_source_han_t *
mucs_source_list_add_source( mucs_source_list_t * source_list, mucs_source_type_t source_type, int count, const void *data_ptr,
                                  const char *delims, const char *eq, const mucs_prefix_encoder_t * pref_ids )
{
  mucs_source_han_t *osrc = NULL;

  osrc = mucs_source_create_setup( source_type, count, data_ptr, delims, eq, pref_ids );
/*if ( !osrc )
    DIE( "FATAL ERROR: can't create \"source\" - %d; %d; %p; %s; %s; %p\n", source_type, count, data_ptr, delims, eq, pref_ids  ); */
  if ( osrc )
    mucs_source_list_add( source_list, osrc );
  return osrc;
}
