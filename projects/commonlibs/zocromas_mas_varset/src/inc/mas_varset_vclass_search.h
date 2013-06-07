#ifndef MAS_VARSET_VCLASS_SEARCH_H
#  define MAS_VARSET_VCLASS_SEARCH_H

int _mas_varset_compare_vclass( const void *a, const void *b );

mas_vclass_element_t *_mas_varset_vclass_find_variable( mas_varset_class_t * vclass, const char *name );
mas_vclass_element_t *mas_varset_vclass_find_variable( mas_varset_class_t * vclass, const char *name );

mas_vclass_element_t *__mas_varset_vclass_search_variable( mas_varset_class_t * vclass, const char *name );
void _mas_varset_vclass_search_variable( mas_varset_class_t * vclass, const char *name, const char *value );
mas_varset_class_t *mas_varset_vclass_search_variable( mas_varset_class_t * vclass, const char *vclass_name, const char *name,
                                                       const char *value );

mas_varset_class_t *mas_varset_vclass_search_variable_va( mas_varset_class_t * vclass, const char *vclass_name, const char *name,
                                                          mas_xvsnprintf_t func, const char *fmt, va_list args );
mas_varset_class_t *mas_varset_vclass_search_variablef( mas_varset_class_t * vclass, const char *vclass_name, const char *name,
                                                        mas_xvsnprintf_t func, const char *fmt, ... );


#endif

