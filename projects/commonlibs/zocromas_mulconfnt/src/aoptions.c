#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "global.h"

#include "option.h"
#include "option_base.h"

#include "option_tablist.h"

#include "aoptions.h"

const mucs_option_t *
mucs_config_option_tabnode_aoptions( const mucs_option_table_list_t * tabnode )
{
  const mucs_option_t *options = NULL;

  if ( tabnode->allocated )
    options = tabnode->voptions;
  else
    options = tabnode->coptions;
  return options;
}

mucs_option_t *
mucs_config_aoptions_clone( const mucs_option_t * options, size_t count )
{
  mucs_option_t *options_clone = mas_calloc( count + 1, sizeof( mucs_option_t ) );

  for ( unsigned no = 0; no < count; no++ )
  {
    options_clone[no] = options[no];
    options_clone[no].s.name = mas_strdup( options[no].s.name );
  }
  return options_clone;
}

void
mucs_config_aoptions_reset( mucs_option_t * options, size_t count )
{
  for ( unsigned no = 0; no < count; no++ )
  {
    mas_free( options[no].s.name );
    options[no].s.name = NULL;
  }
}

void
mucs_config_aoptions_delete( mucs_option_t * options, size_t count )
{
  mucs_config_aoptions_reset( options, count );
  mas_free( options );
}
