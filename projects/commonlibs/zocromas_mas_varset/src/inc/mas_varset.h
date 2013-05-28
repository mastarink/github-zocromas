#ifndef MAS_VARSET_H
#  define MAS_VARSET_H

#  include <search.h>

mas_varset_t *mas_varset_create( void );
void mas_varset_delete( mas_varset_t * obj );

mas_varset_t *mas_varset_search_variable( mas_varset_t * obj, const char *vclass_name, const char *name, const char *value );


void mas_varset_walk_classes( mas_varset_t * varset, void ( *action ) ( const void *nodep, const VISIT which, const int depth ) );

#endif
