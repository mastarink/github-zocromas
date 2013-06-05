#ifndef MAS_VARSET_H
#  define MAS_VARSET_H

#  include <search.h>
#  include <mastar/types/mas_varset_types.h>

mas_varset_t *mas_varset_create( void );
void mas_varset_delete( mas_varset_t * obj );

mas_vclass_element_t *mas_varset_find_variable( mas_varset_t * varset, const char *vclass_name, const char *name );

mas_varset_t *mas_varset_search_variable( mas_varset_t * obj, const char *vclass_name, const char *name, const char *value );
mas_varset_t *mas_varset_search_variablef( mas_varset_t * varset, const char *vclass, const char *name, mas_xvsnprintf_t func,
                                           const char *fmt, ... );
mas_varset_t *mas_varset_search_variable_va( mas_varset_t * varset, const char *vclass, const char *name,
                                             mas_xvsnprintf_t func, const char *fmt, va_list args );


void mas_varset_walk_classes( mas_varset_t * varset, void ( *action ) ( const void *nodep, const VISIT which, const int depth ) );

void mas_varset_write( int fs, mas_varset_t * varset, const char *vclass_name );


mas_varset_t *mas_varset_set_head( mas_varset_t * varset, const char *vclass_name, const char *head );
mas_varset_t *mas_varset_add_tail( mas_varset_t * varset, const char *vclass_name, const char *head );

#endif
