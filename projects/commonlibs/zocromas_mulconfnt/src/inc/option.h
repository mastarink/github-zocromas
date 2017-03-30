#ifndef MUCS_OPTION_H
# define MUCS_OPTION_H

# include "mulconfnt_types.h"

void mucs_config_option_set_value( mucs_option_t * opt, const char *string_value );

unsigned long mucs_config_option_flags( const mucs_option_t * opt );
int mucs_config_option_flag( const mucs_option_t * opt, unsigned long mask );

int mucs_config_option_lookup_option_table( const mucs_option_t * option_table, const char *arg, const char *eq,
                                                             mucs_optscanner_t * optscan );

#endif
