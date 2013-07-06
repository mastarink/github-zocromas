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

#include <mastar/autoobject/mas_autoobject_object.h>
#include <mastar/autoobject/mas_autoobject.h>

#include "mas_autoset_object.h"

#include "mas_autoset_search.h"

mas_autoobject_t *
mas_autoset_search_autoobject( mas_autoset_t * autoset, const char *autoobject_name )
{
  mas_autoobject_t *autoobject = NULL;
  mas_autoobject_t *found = NULL;

  if ( autoset )
  {
    mas_autoobject_t **pfound = NULL;

    autoobject = mas_autoobject_create(  );
    mas_autoobject_set_name( autoobject, autoobject_name );
    if ( autoobject )
    {
      pfound = tsearch( autoobject, &autoset->instances, _mas_autoobject_compare );
      if ( pfound )
        found = *pfound;
      if ( found == autoobject )
      {
        found->id = ++autoset->ninstances;
      }
      else
      {
        mas_autoobject_delete( autoobject );
        autoobject = NULL;
      }
    }
  }
  return found;
}

mas_autoobject_t *
mas_autoset_find_autoobject( mas_autoset_t * autoset, const char *autoobject_name )
{
  mas_autoobject_t *autoobject = NULL;
  mas_autoobject_t *found = NULL;

  if ( autoset )
  {
    mas_autoobject_t **pfound = NULL;

    autoobject = mas_autoobject_create(  );
    mas_autoobject_set_name( autoobject, autoobject_name );
    if ( autoobject )
    {
      pfound = tfind( autoobject, &autoset->instances, _mas_autoobject_compare );
      if ( pfound )
        found = *pfound;
      mas_autoobject_delete( autoobject );
    }
  }
  return found;
}
