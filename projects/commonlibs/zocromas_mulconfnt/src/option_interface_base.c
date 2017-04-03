#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>
#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "source_list_base.h"
#include "option_tablist_base.h"

#include "option.h"

#include "option_interface_base.h"

void
mucs_config_option_interface_init( mucs_option_interface_t * option_interface )
{
  memset( option_interface, 0, sizeof( mucs_option_interface_t ) );
}

mucs_option_interface_t *
mucs_config_option_interface_create( void )
{
  mucs_option_interface_t *option_interface = mas_calloc( 1, sizeof( mucs_option_interface_t ) );

  mucs_config_option_interface_init( option_interface );
  return option_interface;
}

mucs_option_interface_t *
mucs_config_option_interface_create_setup( const char *name, const mucs_option_t * options )
{
  mucs_option_interface_t *interface = mucs_config_option_interface_create(  );

  interface->source_list = mucs_source_list_create(  );
  interface->tablist = mucs_config_option_tablist_add( interface->tablist, name, options, 0 );
  return interface;
}

void
mucs_config_option_interface_reset( mucs_option_interface_t * interface )
{
  if ( interface )
  {
    mucs_source_list_delete( interface->source_list );
    mucs_config_option_tablist_delete( interface->tablist );
  }
}

void
mucs_config_option_interface_delete( mucs_option_interface_t * interface )
{
  mucs_config_option_interface_reset( interface );
  mas_free( interface );
}
