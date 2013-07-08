#ifndef MAS_AUTOSET_H
#  define MAS_AUTOSET_H

#  include <search.h>
#  include <mastar/types/mas_autoset_types.h>

int mas_autoset_delete_autoobject( mas_autoset_t * autoset, const char *instance_name );
void mas_autoset_set_docroot( mas_autoset_t * autoset, const char *docroot );

#endif
