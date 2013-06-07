#ifndef MAS_VARSET_HEADTAIL_H
#  define MAS_VARSET_HEADTAIL_H

#  include <search.h>
#  include <mastar/types/mas_varset_types.h>

mas_varset_t *mas_varset_set_head( mas_varset_t * varset, const char *vclass_name, const char *head );
mas_varset_t *mas_varset_add_tail( mas_varset_t * varset, const char *vclass_name, const char *head );

#endif

