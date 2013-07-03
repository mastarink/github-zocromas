#define MAS_USE_VARVEC
#ifndef MAS_USE_VARVEC

#  include <stdlib.h>
#  include <string.h>

#  include <mastar/wrap/mas_std_def.h>

#  include <mastar/types/mas_varset_types.h>

#  include "mas_varset_vclass.h"

unsigned long
mas_varset_vclass_id( mas_varset_class_t * vclass )
{
  return vclass ? vclass->id : 0;
}

void
mas_varset_vclass_write( int fd, mas_varset_class_t * vclass )
{
  if ( fd > 0 && vclass && vclass->vec )
    ( void ) writev( fd, vclass->vec, vclass->el_cnt );
}

#endif
