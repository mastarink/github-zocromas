#ifndef MAS_VARVEC_NAMEVALUE_H
#  define MAS_VARVEC_NAMEVALUE_H

#  include <mastar/types/mas_varvec_types.h>

const char *mas_varvec_name( mas_varvec_t * vclass );

char *mas_varvec_variable_get_name( mas_varvec_element_t * var );
const char *mas_varvec_variable_get_name_ref( mas_varvec_element_t * var );

char *mas_varvec_variable_get_value( mas_varvec_element_t * var );
const char *mas_varvec_variable_get_value_ref( mas_varvec_element_t * var );

void mas_varvec_variable_set_name( mas_varvec_element_t * var, const char *name );
void mas_varvec_variable_set_value( mas_varvec_element_t * var, const char *value );
void mas_varvec_variable_set_value_va( mas_varvec_element_t * var, mas_xvsnprintf_t func, const char *fmt, va_list args );
void mas_varvec_variable_set_valuef( mas_varvec_element_t * var, mas_xvsnprintf_t func, const char *fmt, ... );



/* #  include <mastar/types/mas_common_cdefs.h> */
#endif
