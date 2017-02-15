#ifndef MULCONF_OPTION_BASE_H
# define MULCONF_OPTION_BASE_H

# include "mulconfnt_types.h"

config_option_t *mucs_config_option_create( void );
config_option_t *mucs_config_option_clone( const config_option_t * topt );
void mucs_config_option_delete( config_option_t * opt );

#endif

