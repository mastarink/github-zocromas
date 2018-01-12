#ifndef MUCS_OPTION_VALUE_H
# define MUCS_OPTION_VALUE_H

# include "mulconfnt_types.h"

void mucs_config_option_ptr_to_nvalue( mucs_option_t * opt );
int mucs_config_option_set_value( mucs_option_t * opt, const char *string_value, mucs_errors_t * perrors );
void mucs_config_option_nvalue_to_ptr( mucs_option_t * opt );

#endif
