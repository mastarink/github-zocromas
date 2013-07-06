#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>

#include <mastar/types/mas_varset_types.h>

#include <mastar/varvec/mas_varvec_object.h>
#include <mastar/varvec/mas_varvec.h>
#include <mastar/varvec/mas_varvec_search.h>

#include "mas_varset_search.h"
#include "mas_varset.h"

int
mas_varset_delete_vclass( mas_varset_t * varset, const char *vclass_name )
{
  mas_varvec_t *vclass = NULL;
  mas_varvec_t *found = NULL;

  if ( varset )
  {
    mas_varvec_t **pfound = NULL;

    vclass = mas_varvec_create( vclass_name );
    if ( vclass )
    {
      pfound = tfind( vclass, &varset->classes, _mas_varvec_compare );
      if ( pfound )
      {
        found = *pfound;
        tdelete( vclass, &varset->classes, _mas_varvec_compare );
        if ( found != vclass )
        {
          mas_varvec_delete( found );
        }
      }
      mas_varvec_delete( vclass );
      vclass = NULL;
    }
  }
  return found ? 1 : 0;
}

void
mas_varset_write( int fd, mas_varset_t * varset, const char *vclass_name )
{
  if ( varset )
  {
    mas_varvec_t *vclass = NULL;

    vclass = mas_varset_find_vclass( varset, vclass_name );
    if ( vclass )
      mas_varvec_write( fd, vclass );
  }
}
