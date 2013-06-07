#ifndef MAS_VARSET_VCLASS_ELEMENT_H
#  define MAS_VARSET_VCLASS_ELEMENT_H


mas_vclass_element_t *_mas_varset_vclass_add_elements( mas_varset_class_t * vclass, size_t num );
mas_vclass_element_t *_mas_varset_vclass_element( mas_varset_class_t * vclass, size_t num );


void mas_varset_vclass_variable_set_element( mas_vclass_element_t * var, size_t offset, const char *suffix, size_t suffix_len,
                                             const char *str );
#endif

