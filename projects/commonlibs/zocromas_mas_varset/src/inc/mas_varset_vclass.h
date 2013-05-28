#ifndef MAS_VARSET_VCLASS_H
#  define MAS_VARSET_VCLASS_H

mas_varset_class_t *mas_varset_find_vclass( mas_varset_t * varset, const char *vclass_name );
mas_varset_class_t *mas_varset_search_vclass( mas_varset_t * varset, const char *vclass_name );
int mas_varset_delete_vclass( mas_varset_t * varset, const char *vclass_name );

mas_varset_class_t *mas_varset_vclass_create( const char *vclass_name );
void mas_varset_vclass_delete( mas_varset_class_t * vclass );

const char *mas_varset_vclass_name( mas_varset_class_t * vclass );
unsigned long mas_varset_vclass_id( mas_varset_class_t * vclass );

mas_var_t *mas_varset_vclass_find_variable( mas_varset_class_t * vclass, const char *name );
mas_var_t *mas_varset_vclass_search_variable( mas_varset_class_t * vclass, const char *name );

char *mas_varset_vclass_variable_get_name( mas_var_t * var );
const char *mas_varset_vclass_variable_get_name_ref( mas_var_t * var );

char *mas_varset_vclass_variable_get_value( mas_var_t * var );
const char *mas_varset_vclass_variable_get_value_ref( mas_var_t * var );

void mas_varset_vclass_variable_set_name( mas_var_t * var, const char *name );
void mas_varset_vclass_variable_set_value( mas_var_t * var, const char *value );

void mas_varset_vclass_write( int fd, mas_varset_class_t * vclass );

#endif
