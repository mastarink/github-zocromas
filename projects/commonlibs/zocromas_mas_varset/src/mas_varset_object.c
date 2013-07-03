#define MAS_USE_VARVEC

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

#ifdef MAS_USE_VARVEC
#  include <mastar/varvec/mas_varvec_object.h>
#  include <mastar/varvec/mas_varvec.h>
#  include <mastar/varvec/mas_varvec_search.h>
#  include <mastar/varvec/mas_varvec_namevalue.h>
#  include <mastar/varvec/mas_varvec_headtail.h>
#else
#  include "mas_varset_vclass_object.h"
#  include "mas_varset_vclass.h"
#  include "mas_varset_vclass_search.h"
#  include "mas_varset_vclass_namevalue.h"
#  include "mas_varset_vclass_headtail.h"
#endif

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

#ifdef MAS_USE_VARVEC
static void
_mas_varvec_delete( void *vclass )
{
  mas_varvec_delete( ( mas_varvec_t * ) vclass );
}
#else
static void
_mas_varset_vclass_delete( void *vclass )
{
  mas_varset_vclass_delete( ( mas_varset_class_t * ) vclass );
}
#endif

void
mas_varset_delete( mas_varset_t * varset )
{
  if ( varset )
  {
#ifdef MAS_USE_VARVEC
    tdestroy( varset->classes, _mas_varvec_delete );
#else
    tdestroy( varset->classes, _mas_varset_vclass_delete );
#endif
    mas_free( varset );
    varset = NULL;
  }
}
