#include <stdlib.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/autoobject/mas_autoobject_object.h>
#include <mastar/autoobject/mas_autoobject.h>

#include "mas_autoset_search.h"
#include "mas_autoset.h"

int
mas_autoset_delete_autoobject( mas_autoset_t * autoset, const char *instance_name )
{
  mas_autoobject_t *autoobject = NULL;
  mas_autoobject_t *found = NULL;

  if ( autoset )
  {
    mas_autoobject_t **pfound = NULL;

    autoobject = mas_autoobject_create(  );
    mas_autoobject_set_name( autoobject, autoset->docroot, instance_name );
    if ( autoobject )
    {
      pfound = tfind( autoobject, &autoset->instances, _mas_autoobject_compare );
      if ( pfound )
      {
        found = *pfound;
        tdelete( autoobject, &autoset->instances, _mas_autoobject_compare );
        if ( found != autoobject )
        {
          mas_autoobject_delete( found );
        }
      }
      mas_autoobject_delete( autoobject );
      autoobject = NULL;
    }
  }
  return found ? 1 : 0;
}

void
mas_autoset_set_docroot( mas_autoset_t * autoset, const char *docroot )
{
  if ( autoset )
  {
    if ( autoset->docroot )
      mas_free( autoset->docroot );
    autoset->docroot = mas_strdup( docroot );
  }
}
