#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>

#include "mas_varset_types.h"
#include "mas_varset_vclass.h"
#include "mas_varset.h"

mas_varset_t *
mas_varset_create( void )
{
  mas_varset_t *varset = NULL;

  varset = mas_malloc( sizeof( mas_varset_t ) );
  if ( varset )
  {
    memset( varset, 0, sizeof( mas_varset_t ) );
  }
  return varset;
}

static void
_mas_varset_vclass_delete( void *vclass )
{
  mas_varset_vclass_delete( ( mas_varset_class_t * ) vclass );
}

void
mas_varset_delete( mas_varset_t * varset )
{
  if ( varset )
  {
    tdestroy( varset->classes, _mas_varset_vclass_delete );
    mas_free( varset );
    varset = NULL;
  }
}

void
mas_varset_walk_classes( mas_varset_t * varset, void ( *action ) ( const void *nodep, const VISIT which, const int depth ) )
{
  if ( varset )
  {
    twalk( varset->classes, action );
  }
}

mas_varset_t *
mas_varset_search_variable( mas_varset_t * varset, const char *vclass_name, const char *name, const char *value )
{
  if ( !varset )
    varset = mas_varset_create(  );
  if ( varset )
  {
    mas_varset_class_t *vclass __attribute__ ( ( unused ) ) = NULL;

    vclass = mas_varset_search_vclass( varset, vclass_name );
    if ( vclass )
    {
      mas_var_t *v;

      v = mas_varset_vclass_search_variable( vclass, name );
      if ( v )
      {
        mas_varset_vclass_variable_set_value( v, value );
      }
    }
  }
  return varset;
}
