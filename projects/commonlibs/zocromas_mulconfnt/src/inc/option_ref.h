#ifndef MUCS_OPTION_REF_H
# define MUCS_OPTION_REF_H

int mucs_config_option_npos( mucs_option_t * opt );
const char *mucs_config_option_string_value( mucs_option_t * opt );
void mucs_config_option_set_string_value_na( mucs_option_t * opt, char *string_value );
void mucs_config_option_set_string_value( mucs_option_t * opt, const char *string_value );

#endif
