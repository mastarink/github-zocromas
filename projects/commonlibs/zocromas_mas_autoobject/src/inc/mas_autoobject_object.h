#ifndef MAS_AUTOOBJECT_OBJECT_H
#  define MAS_AUTOOBJECT_OBJECT_H

#  include <mastar/types/mas_autoobject_types.h>

void _mas_autoobject_set_name( mas_autoobject_t * obj, const char *docroot, const char *name );
mas_autoobject_t *mas_autoobject_create( void );
void mas_autoobject_delete( mas_autoobject_t * obj );

#endif
