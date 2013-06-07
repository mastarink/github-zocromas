#ifndef MAS_VARSET_H
#  define MAS_VARSET_H

#  include <search.h>
#  include <mastar/types/mas_varset_types.h>

int mas_varset_delete_vclass( mas_varset_t * varset, const char *vclass_name );

void mas_varset_write( int fs, mas_varset_t * varset, const char *vclass_name );

#endif
