#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"

#include "source_base.h"
#include "source.h"
#include "source_list.h"

void
mulconfnt_source_list_integrate( config_source_list_t * source_list, int pos )
{
  for ( config_source_desc_t * osrc = source_list->first; osrc; osrc = osrc->next )
  {
    mulconfnt_source_load_targ( osrc, pos );
    mas_add_argvc_argvc( &source_list->targ, &osrc->targ, 0 );
  }
}

static void
mulconfnt_source_list_add( config_source_list_t * source_list, config_source_desc_t * osrc )
{
  if ( source_list && osrc )
  {
    config_source_desc_t *os = source_list->first;

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

config_source_desc_t *
mulconfnt_source_list_add_source( config_source_list_t * source_list, config_source_t source_type, int count, const void *data_ptr,
                                  const char *delims, const char *eq, const config_prefix_encoder_t * pref_ids )
{
  config_source_desc_t *osrc = NULL;

  osrc = mulconfnt_source_create_setup( source_type, count, data_ptr, delims, eq, pref_ids );
/*if ( !osrc )
    DIE( "FATAL ERROR: can't create \"source\" - %d; %d; %p; %s; %s; %p\n", source_type, count, data_ptr, delims, eq, pref_ids  ); */
  if ( osrc )
    mulconfnt_source_list_add( source_list, osrc );
  return osrc;
}
