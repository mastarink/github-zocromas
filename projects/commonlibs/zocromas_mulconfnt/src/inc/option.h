#ifndef MUCS_OPTION_H
# define MUCS_OPTION_H

# include "mulconfnt_types.h"

void mucs_config_option_ptr_to_nvalue( mucs_option_t * opt );
void mucs_config_option_nvalue_to_ptr( mucs_option_t * opt );

static inline int
mucs_config_option_check( const mucs_option_t * opt )
{
  return opt ? 1 : 0;
}

int mucs_config_option_set_value( mucs_option_t * opt, const char *string_value );

mucs_flags_t mucs_config_option_flags( const mucs_option_t * opt );
int mucs_config_option_flag( const mucs_option_t * opt, mucs_flags_t mask );

int mucs_config_option_lookup_option_table( const mucs_option_t * option_table, const char *arg, const char *eq, mucs_optscanner_t * optscan );

#endif
