#ifndef MAS_VARSET_VCLASS_NAMEVALUE_H
#  define MAS_VARSET_VCLASS_NAMEVALUE_H

const char *mas_varset_vclass_name( mas_varset_class_t * vclass );

char *mas_varset_vclass_variable_get_name( mas_vclass_element_t * var );
const char *mas_varset_vclass_variable_get_name_ref( mas_vclass_element_t * var );

char *mas_varset_vclass_variable_get_value( mas_vclass_element_t * var );
const char *mas_varset_vclass_variable_get_value_ref( mas_vclass_element_t * var );

void mas_varset_vclass_variable_set_name( mas_vclass_element_t * var, const char *name );
void mas_varset_vclass_variable_set_value( mas_vclass_element_t * var, const char *value );
void mas_varset_vclass_variable_set_value_va( mas_vclass_element_t * var, mas_xvsnprintf_t func, const char *fmt, va_list args );
void mas_varset_vclass_variable_set_valuef( mas_vclass_element_t * var, mas_xvsnprintf_t func, const char *fmt, ... );

#endif

