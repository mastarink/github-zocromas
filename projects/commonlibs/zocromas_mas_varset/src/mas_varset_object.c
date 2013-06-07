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

#include <mastar/types/mas_varset_types.h>
#include "mas_varset_vclass_object.h"
#include "mas_varset_vclass.h"
#include "mas_varset_vclass_search.h"
#include "mas_varset_vclass_namevalue.h"
#include "mas_varset_vclass_headtail.h"


#include "mas_varset_object.h"

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


