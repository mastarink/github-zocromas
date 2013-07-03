#define MAS_USE_VARVEC
#ifndef MAS_USE_VARVEC

#  include <stdlib.h>
#  include <string.h>

#  include <mastar/wrap/mas_std_def.h>

#  include <mastar/wrap/mas_memory.h>

#  include <mastar/types/mas_varset_types.h>

#  include "mas_varset_vclass_object.h"




mas_varset_class_t *
mas_varset_vclass_create( const char *vclass_name )
{
  mas_varset_class_t *vclass = NULL;

  vclass = mas_malloc( sizeof( mas_varset_class_t ) );
  if ( vclass )
  {
    memset( vclass, 0, sizeof( mas_varset_class_t ) );
    if ( vclass_name )
      vclass->name = mas_strdup( vclass_name );
  }
  return vclass;
}

void
mas_varset_vclass_delete( mas_varset_class_t * vclass )
{
  if ( vclass )
  {
    if ( vclass->el_cnt && vclass->vec )
    {
      mas_vclass_element_t *vec;

      vec = vclass->vec;
      for ( int j = 0; j < vclass->el_cnt; j++ )
      {
        char *v;

        v = vec[j].iov_base;
        vec[j].iov_base = NULL;
        if ( v )
          mas_free( v );
      }
      if ( vec )
        mas_free( vclass->vec );
    }
    if ( vclass->name )
    {
      mas_free( vclass->name );
      vclass->name = NULL;
    }
    mas_free( vclass );
    vclass = NULL;
  }
}

#endif
