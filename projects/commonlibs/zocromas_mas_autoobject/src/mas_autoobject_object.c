#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/sendfile.h>



#include <mastar/wrap/mas_std_def.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_tools.h>


#include "mas_autoobject_object.h"

void
_mas_autoobject_set_name( mas_autoobject_t * obj, const char *docroot, const char *name )
{
  if ( obj )
  {
    if ( obj->docroot )
      mas_free( obj->docroot );
    obj->docroot = NULL;
    if ( docroot )
      obj->docroot = mas_strdup( docroot );

    if ( obj->name )
      mas_free( obj->name );
    if ( obj->gtime )
      mas_free( obj->gtime );
    if ( obj->etag )
      mas_free( obj->etag );
    obj->name = NULL;
    if ( name )
      obj->name = mas_strdup( name );
  }
}

mas_autoobject_t *
mas_autoobject_create( void )
{
  mas_autoobject_t *obj = NULL;

  obj = mas_malloc( sizeof( mas_autoobject_t ) );
  memset( obj, 0, sizeof( mas_autoobject_t ) );
  return obj;
}

void
mas_autoobject_delete( mas_autoobject_t * obj )
{
  if ( obj )
  {
    _mas_autoobject_set_name( obj, NULL, NULL );
    mas_free( obj );
  }
}
