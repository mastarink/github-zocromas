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

#include "source_base.h"
#include "source.h"
#include "source_list.h"

void
mulconfnt_source_list_integrate( config_source_list_t * source_list )
{
  for ( config_source_desc_t * osrc = source_list->first; osrc; osrc = osrc->next )
  {
    mulconfnt_source_load_targ( osrc );
    mas_add_argvc_argvc( &source_list->targ, &osrc->targ, 0 );
  }
}

static void
mulconfnt_source_list_add( config_source_list_t * source_list, config_source_desc_t * osrc )
{
  if ( source_list )
  {
    if ( source_list->first )
    {
      config_source_desc_t *os = source_list->first;

      while ( os && os->next )
      {
        os = os->next;
      }
      os->next = osrc;
    }
    else
      source_list->first = osrc;
  }
}

config_source_desc_t *
mulconfnt_source_list_add_source( config_source_list_t * source_list, config_source_t source_type, int count, void *data_ptr, const char *delims,
                                  const char *eq, const config_prefix_encoder_t * pref_ids )
{
  config_source_desc_t *osrc = NULL;

  osrc = mulconfnt_source_create_set( source_type, count, data_ptr, delims, eq, pref_ids );
  mulconfnt_source_list_add( source_list, osrc );
  return osrc;
}
