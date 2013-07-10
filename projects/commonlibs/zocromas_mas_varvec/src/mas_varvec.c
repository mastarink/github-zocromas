#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>

#include "mas_varvec.h"

unsigned long
mas_varvec_id( mas_varvec_t * vclass )
{
  return vclass ? vclass->id : 0;
}

ssize_t
mas_varvec_write( int fd, mas_varvec_t * vclass )
{
  ssize_t r = -1;

  if ( fd > 0 && vclass && vclass->vec )
    r = writev( fd, vclass->vec, vclass->el_cnt );
  return r;
}
