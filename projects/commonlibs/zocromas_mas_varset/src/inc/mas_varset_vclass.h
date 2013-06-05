#ifndef MAS_VARSET_VCLASS_H
#  define MAS_VARSET_VCLASS_H

mas_varset_class_t *mas_varset_find_vclass( mas_varset_t * varset, const char *vclass_name );
mas_varset_class_t *mas_varset_search_vclass( mas_varset_t * varset, const char *vclass_name );
int mas_varset_delete_vclass( mas_varset_t * varset, const char *vclass_name );

mas_varset_class_t *mas_varset_vclass_create( const char *vclass_name );
void mas_varset_vclass_delete( mas_varset_class_t * vclass );

mas_vclass_element_t *_mas_varset_vclass_add_elements( mas_varset_class_t * vclass, size_t num );
void _mas_varset_vclass_add_tail( mas_varset_class_t * vclass, const char *tail );
mas_varset_class_t *mas_varset_vclass_add_tail( mas_varset_class_t * vclass, const char *vclass_name, const char *tail );

void _mas_varset_vclass_set_head( mas_varset_class_t * vclass, const char *head );
mas_varset_class_t *mas_varset_vclass_set_head( mas_varset_class_t * vclass, const char *vclass_name, const char *head );
mas_varset_class_t *mas_varset_vclass_set_head_va( mas_varset_class_t * vclass, const char *vclass_name, mas_xvsnprintf_t func,
                                                   const char *fmt, va_list args );
mas_varset_class_t *mas_varset_vclass_set_headf( mas_varset_class_t * vclass, const char *vclass_name, mas_xvsnprintf_t func,
                                                 const char *fmt, ... );

mas_vclass_element_t *_mas_varset_vclass_element( mas_varset_class_t * vclass, size_t num );

const char *mas_varset_vclass_name( mas_varset_class_t * vclass );
unsigned long mas_varset_vclass_id( mas_varset_class_t * vclass );

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

char *mas_varset_vclass_variable_get_name( mas_vclass_element_t * var );
const char *mas_varset_vclass_variable_get_name_ref( mas_vclass_element_t * var );

char *mas_varset_vclass_variable_get_value( mas_vclass_element_t * var );
const char *mas_varset_vclass_variable_get_value_ref( mas_vclass_element_t * var );

/* mas_vclass_element_t *mas_varset_vclass_set_element( mas_varset_class_t * vclass, size_t offset, const char *suffix, size_t suffix_len,    */
/*                                                      const char *str );                                                                    */
/* mas_vclass_element_t *mas_varset_vclass_set_element_va( mas_varset_class_t * vclass, size_t offset, const char *suffix, size_t suffix_len, */
/*                                                         mas_xvsnprintf_t func, const char *fmt, va_list args );                            */
/* mas_vclass_element_t *mas_varset_vclass_set_elementf( mas_varset_class_t * vclass, size_t offset, const char *suffix, size_t suffix_len,   */
/*                                                       mas_xvsnprintf_t func, const char *fmt, ... );                                       */

void mas_varset_vclass_variable_set_element( mas_vclass_element_t * var, size_t offset, const char *suffix, size_t suffix_len,
                                             const char *str );
void mas_varset_vclass_variable_set_name( mas_vclass_element_t * var, const char *name );
void mas_varset_vclass_variable_set_value( mas_vclass_element_t * var, const char *value );
void mas_varset_vclass_variable_set_value_va( mas_vclass_element_t * var, mas_xvsnprintf_t func, const char *fmt, va_list args );
void mas_varset_vclass_variable_set_valuef( mas_vclass_element_t * var, mas_xvsnprintf_t func, const char *fmt, ... );

void mas_varset_vclass_write( int fd, mas_varset_class_t * vclass );

#endif
