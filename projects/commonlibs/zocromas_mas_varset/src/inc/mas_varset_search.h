#ifndef MAS_VARSET_SEARCH_H
#  define MAS_VARSET_SEARCH_H

#  include <search.h>
#  include <mastar/types/mas_varvec_types.h>
#  include <mastar/types/mas_varset_types.h>

void mas_varset_walk_classes( mas_varset_t * varset, void ( *action ) ( const void *nodep, const VISIT which, const int depth ) );


mas_varvec_element_t *mas_varset_find_variable( mas_varset_t * varset, const char *vclass_name, const char *name );

mas_varset_t *mas_varset_search_variable( mas_varset_t * obj, const char *vclass_name, const char *name, const char *value );
mas_varset_t *mas_varset_search_variablef( mas_varset_t * varset, const char *vclass, const char *name, mas_xvsnprintf_t func,
                                           const char *fmt, ... );
mas_varset_t *mas_varset_search_variable_va( mas_varset_t * varset, const char *vclass, const char *name,
                                             mas_xvsnprintf_t func, const char *fmt, va_list args );

mas_varvec_t *mas_varset_find_vclass( mas_varset_t * varset, const char *vclass_name );
mas_varvec_t *mas_varset_search_vclass( mas_varset_t * varset, const char *vclass_name );



/* #include <mastar/types/mas_common_cdefs.h> */
#endif
