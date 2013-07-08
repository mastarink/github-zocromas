#ifndef MAS_AUTOSET_SEARCH_H
#  define MAS_AUTOSET_SEARCH_H

#  include <search.h>
#  include <mastar/types/mas_autoobject_types.h>
#  include <mastar/types/mas_autoset_types.h>


mas_autoobject_t *mas_autoset_search_autoobject( mas_autoset_t * autoset, const char *docroot, const char *autoobject_name );
mas_autoobject_t *mas_autoset_find_autoobject( mas_autoset_t * autoset, const char *docroot, const char *autoobject_name );

#endif
