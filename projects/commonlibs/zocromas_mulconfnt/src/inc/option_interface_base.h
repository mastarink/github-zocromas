#ifndef MUCS_OPTION_INTERFACE_BASE_H
# define MUCS_OPTION_INTERFACE_BASE_H

# include "mulconfnt_types.h"

void mucs_config_option_interface_init( mucs_option_interface_t * option_interface );
mucs_option_interface_t *mucs_config_option_interface_create( void );
mucs_option_interface_t *mucs_config_option_interface_create_setup( const char *name, const mucs_option_t * options, int special_options );

void mucs_config_option_interface_reset( mucs_option_interface_t * interface );
void mucs_config_option_interface_delete( mucs_option_interface_t * interface );

mucs_option_interface_t *mucs_config_option_interface_tabnode_add( mucs_option_interface_t * interface, const char *name,
                                                                   const mucs_option_t * options );

#endif
