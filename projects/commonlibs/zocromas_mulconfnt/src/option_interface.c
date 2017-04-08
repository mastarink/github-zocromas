#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>
#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "source_list.h"

#include "option.h"

#include "option_interface.h"

void
mucs_option_interface_add_source( mucs_option_interface_t * interface, mucs_source_type_t source_type, int count, const void *data_ptr )
{
  mucs_source_list_add_source( interface->source_list, source_type, count, data_ptr );
}

int
mucs_option_interface_lookup_source_list( mucs_option_interface_t * interface, mucs_source_list_t * source_list )
{
  rDECLBAD;
  rC( mucs_source_list_lookup_all( source_list, interface->tablist ) );
  rRET;
}

int
mucs_option_interface_lookup_all( mucs_option_interface_t * interface )
{
  rDECLBAD;
  rC( mucs_source_list_lookup_all( interface->source_list, interface->tablist ) );
  rRET;
}
