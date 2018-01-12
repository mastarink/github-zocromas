#ifndef MUCS_OPTION_FLAGS_H
# define MUCS_OPTION_FLAGS_H

# include "mulconfnt_types.h"


mucs_flags_t mucs_config_option_flags( const mucs_option_t * opt );

int mucs_config_soption_flag( const mucs_option_static_t * sopt, mucs_flags_t mask );
int mucs_config_option_flag( const mucs_option_t * opt, mucs_flags_t mask );

#endif
