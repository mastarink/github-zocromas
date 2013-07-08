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

#include <mastar/types/mas_autoset_types.h>

#include <mastar/autoobject/mas_autoobject.h>

#include <mastar/autoobject/mas_autoobject_object.h>
/* #include <mastar/autoobject/mas_autoobject_search.h>    */
/* #include <mastar/autoobject/mas_autoobject_namevalue.h> */
/* #include <mastar/autoobject/mas_autoobject_headtail.h>  */

#include "mas_autoset_object.h"

mas_autoset_t *
mas_autoset_create( void )
{
  mas_autoset_t *autoset = NULL;

  autoset = mas_malloc( sizeof( mas_autoset_t ) );
  if ( autoset )
  {
    memset( autoset, 0, sizeof( mas_autoset_t ) );
  }
  return autoset;
}

static void
_mas_autoobject_delete( void *tautoset )
{
  mas_autoobject_delete( ( mas_autoobject_t * ) tautoset );
}

void
mas_autoset_delete( mas_autoset_t * autoset )
{
  if ( autoset )
  {
    tdestroy( autoset->instances, _mas_autoobject_delete );
    if ( autoset->docroot )
      mas_free( autoset->docroot );
    mas_free( autoset );
    autoset = NULL;
  }
}
