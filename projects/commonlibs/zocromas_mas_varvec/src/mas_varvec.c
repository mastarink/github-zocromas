#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>

#include <mastar/types/mas_varvec_types.h>

#include "mas_varvec.h"

unsigned long
mas_varvec_id( mas_varvec_t * vclass )
{
  return vclass ? vclass->id : 0;
}

void
mas_varvec_write( int fd, mas_varvec_t * vclass )
{
  if ( fd > 0 && vclass && vclass->vec )
    ( void ) writev( fd, vclass->vec, vclass->el_cnt );
}
