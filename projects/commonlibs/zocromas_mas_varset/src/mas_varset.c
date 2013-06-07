#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>

#include <mastar/types/mas_varset_types.h>
#include "mas_varset_vclass_object.h"
#include "mas_varset_vclass.h"
#include "mas_varset_vclass_search.h"

#include "mas_varset_search.h"

#include "mas_varset.h"

int
mas_varset_delete_vclass( mas_varset_t * varset, const char *vclass_name )
{
  mas_varset_class_t *vclass = NULL;
  mas_varset_class_t *found = NULL;

  if ( varset )
  {
    mas_varset_class_t **pfound = NULL;

    vclass = mas_varset_vclass_create( vclass_name );
    if ( vclass )
    {
      pfound = tfind( vclass, &varset->classes, _mas_varset_compare_vclass );
      if ( pfound )
      {
        found = *pfound;
        tdelete( vclass, &varset->classes, _mas_varset_compare_vclass );
        if ( found != vclass )
        {
          mas_varset_vclass_delete( found );
        }
      }
      mas_varset_vclass_delete( vclass );
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
    mas_varset_class_t *vclass = NULL;

    vclass = mas_varset_find_vclass( varset, vclass_name );
    if ( vclass )
    {
      mas_varset_vclass_write( fd, vclass );
    }
  }
}
