#ifndef MULCONF_OPTION_BASE_H
# define MULCONF_OPTION_BASE_H

# include "mulconfnt_types.h"

mucs_option_han_t *mucs_config_option_create( void );
mucs_option_han_t *mucs_config_option_clone( const mucs_option_han_t * topt );
void mucs_config_option_delete( mucs_option_han_t * opt );

#endif

