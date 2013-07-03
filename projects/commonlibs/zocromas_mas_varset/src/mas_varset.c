#define MAS_USE_VARVEC

#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>

#include <mastar/types/mas_varset_types.h>
#ifdef MAS_USE_VARVEC
#  include <mastar/varvec/mas_varvec_object.h>
#  include <mastar/varvec/mas_varvec.h>
#  include <mastar/varvec/mas_varvec_search.h>
#else
#  include "mas_varset_vclass_object.h"
#  include "mas_varset_vclass.h"
#  include "mas_varset_vclass_search.h"
#endif
#include "mas_varset_search.h"

#include "mas_varset.h"

int
mas_varset_delete_vclass( mas_varset_t * varset, const char *vclass_name )
{
#ifdef MAS_USE_VARVEC
  mas_varvec_t *vclass = NULL;
  mas_varvec_t *found = NULL;
#else
  mas_varset_class_t *vclass = NULL;
  mas_varset_class_t *found = NULL;
#endif
  if ( varset )
  {
#ifdef MAS_USE_VARVEC
    mas_varvec_t **pfound = NULL;

    vclass = mas_varvec_create( vclass_name );
#else
    mas_varset_class_t **pfound = NULL;

    vclass = mas_varset_vclass_create( vclass_name );
#endif
    if ( vclass )
    {
#ifdef MAS_USE_VARVEC
      pfound = tfind( vclass, &varset->classes, _mas_varvec_compare );
#else
      pfound = tfind( vclass, &varset->classes, _mas_varset_compare_vclass );
#endif
      if ( pfound )
      {
        found = *pfound;
#ifdef MAS_USE_VARVEC
        tdelete( vclass, &varset->classes, _mas_varvec_compare );
#else
        tdelete( vclass, &varset->classes, _mas_varset_compare_vclass );
#endif
        if ( found != vclass )
        {
#ifdef MAS_USE_VARVEC
          mas_varvec_delete( found );
#else
          mas_varset_vclass_delete( found );
#endif
        }
      }
#ifdef MAS_USE_VARVEC
      mas_varvec_delete( vclass );
#else
      mas_varset_vclass_delete( vclass );
#endif
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
#ifdef MAS_USE_VARVEC
    mas_varvec_t *vclass = NULL;
    vclass = mas_varset_find_vclass( varset, vclass_name );
#else
    mas_varset_class_t *vclass = NULL;
    vclass = mas_varset_find_vclass( varset, vclass_name );
#endif

    if ( vclass )
    {
#ifdef MAS_USE_VARVEC
      mas_varvec_write( fd, vclass );
#else
      mas_varset_vclass_write( fd, vclass );
#endif
    }
  }
}
