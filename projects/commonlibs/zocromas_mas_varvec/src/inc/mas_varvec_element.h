#ifndef MAS_VARVEC_ELEMENT_H
#  define MAS_VARVEC_ELEMENT_H

#include <mastar/types/mas_varvec_types.h>

mas_varvec_element_t *_mas_varvec_add_elements( mas_varvec_t * vclass, size_t num );
mas_varvec_element_t *_mas_varvec_element( mas_varvec_t * vclass, size_t num );


void mas_varvec_variable_set_element( mas_varvec_element_t * var, size_t offset, const char *suffix, size_t suffix_len,
                                             const char *str );
#endif

