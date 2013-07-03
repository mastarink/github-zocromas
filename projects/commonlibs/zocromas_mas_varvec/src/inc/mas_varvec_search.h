#ifndef MAS_VARVEC_SEARCH_H
#  define MAS_VARVEC_SEARCH_H

#include <mastar/types/mas_varvec_types.h>

int _mas_varvec_compare( const void *a, const void *b );

mas_varvec_element_t *_mas_varvec_find_variable( mas_varvec_t * vclass, const char *name );
mas_varvec_element_t *mas_varvec_find_variable( mas_varvec_t * vclass, const char *name );

mas_varvec_element_t *__mas_varvec_search_variable( mas_varvec_t * vclass, const char *name );
void _mas_varvec_search_variable( mas_varvec_t * vclass, const char *name, const char *value );
mas_varvec_t *mas_varvec_search_variable( mas_varvec_t * vclass, const char *vclass_name, const char *name,
                                                       const char *value );

mas_varvec_t *mas_varvec_search_variable_va( mas_varvec_t * vclass, const char *vclass_name, const char *name,
                                                          mas_xvsnprintf_t func, const char *fmt, va_list args );
mas_varvec_t *mas_varvec_search_variablef( mas_varvec_t * vclass, const char *vclass_name, const char *name,
                                                        mas_xvsnprintf_t func, const char *fmt, ... );


#endif

